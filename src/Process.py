import sys
import bpy

class Process:
	session = None
	size = 256

	def __init__(self, session):
		self.session = session

		for obj in bpy.data.objects:
			if obj.type != 'MESH':
				continue

			self.object(obj)

		for obj in bpy.data.objects:
			if obj.type != 'MESH':
				continue

			bpy.context.scene.cycles.samples = 24
			bpy.context.scene.cycles.use_square_samples = True
			self.bake(obj)

	def selectObj(self, obj):
		bpy.ops.object.select_all(action='DESELECT')

		if obj != None:
			if self.session.version <= 279:
				obj.select = True
			else:
				obj.select_set(state=True)

		if self.session.version <= 279:
			bpy.context.scene.objects.active = obj
		else:
			bpy.context.view_layer.objects.active = obj

	def bake(self, obj):
		print("Baking")

		if obj.data.uv_layers.active == None:
			print("Ignoring Mesh: " + obj.name)
			return

		self.selectObj(obj)
		bpy.ops.object.bake(type='SHADOW')
		self.selectObj(None)

	def object(self, obj):
		print("Processing Mesh: " + obj.name)

		if len(obj.data.uv_layers) < 1:
			print("Ignoring Mesh (No UV layers): " + obj.name)
			return

		if obj.data.uv_layers.active == None:
			print("Ignoring Mesh (UV Layer in invalid state): " + obj.name)
			return

		tcuv = obj.data.uv_layers.active

		if self.session.version <= 279:
			lmuv = obj.data.uv_textures.new(name="LightMap")
			obj.data.uv_textures.active = lmuv
			lmuv = obj.data.uv_layers[len(obj.data.uv_layers) - 1]
		else:
			lmuv = obj.data.uv_layers.new(name="LightMap")
			obj.data.uv_layers.active = lmuv

		self.selectObj(obj)
		bpy.ops.uv.lightmap_pack(PREF_CONTEXT='ALL_FACES', PREF_PACK_IN_ONE=False)
		self.selectObj(None)

		lightMap = bpy.data.images.new("LightMap_" + obj.name, width=self.size, height=self.size)

		sg = ShadowGroup()
		sg.texture = lightMap
		sg.obj = obj
		self.session.shadowGroups.append(sg)

		for m in obj.material_slots:
			self.material(m.material, sg)

		for face in obj.data.polygons:
			self.face(face, sg, tcuv.data, lmuv.data)

	def face(self, face, sg, uvdat, lmdat):
		mg = None
		mat = sg.obj.material_slots[face.material_index].material

		for m in sg.materialGroups:
			if m.mat == mat:
				mg = m

		if mg == None:
			#raise Exception("Invalid group: " + mat.name)
			print("Ignoring face in " + sg.obj.name + " (No material found)")
			return

		shape = len(face.vertices)

		a = sg.obj.data.vertices[face.vertices[0]]
		b = sg.obj.data.vertices[face.vertices[1]]
		c = sg.obj.data.vertices[face.vertices[2]]

		sf = SimpleFace()
		sf.normal = face.normal
		sf.pa = a.co
		sf.pb = b.co
		sf.pc = c.co
		sf.na = a.normal
		sf.nb = b.normal
		sf.nc = c.normal

		sf.ta = uvdat[face.loop_start + 0].uv
		sf.tb = uvdat[face.loop_start + 1].uv
		sf.tc = uvdat[face.loop_start + 2].uv

		sf.la = lmdat[face.loop_start + 0].uv
		sf.lb = lmdat[face.loop_start + 1].uv
		sf.lc = lmdat[face.loop_start + 2].uv

		if shape == 4:
			d = sg.obj.data.vertices[face.vertices[3]]
			sf.quad = True
			sf.pd = d.co
			sf.nd = d.normal
			sf.td = uvdat[face.loop_start + 3].uv
			sf.ld = lmdat[face.loop_start + 3].uv

		mg.faces.append(sf)

	def material(self, mat, sg):
		print("  Preparing Material [" + mat.name + "] in " + sg.obj.name)

		if mat.use_nodes == False:
			mat.use_nodes = True
			#print("    Ignoring: Does not use nodes")
			#return

		mg = MaterialGroup()
		mg.mat = mat
		nodes = mat.node_tree.nodes

		bpy.ops.image.new(name="tmp", width=1, height=1, color=(1, 1, 1, 1))
		tex = bpy.data.images['tmp']

		for node in nodes:
			if node.type == 'TEX_IMAGE':
				mg.texture = node.image
				node.image = tex
				break

		if mg.texture != None:
			sg.materialGroups.append(mg)
			#print("    Ignoring: Does not have a texture")
			#return

		tiNode = nodes.new('ShaderNodeTexImage')
		tiNode.image = sg.texture
		mat.node_tree.nodes.active = tiNode

