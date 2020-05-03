import sys;
import bpy;

class Clean:
	def __init__(self, session):
		self.session = session

		for obj in bpy.data.objects:
			if obj.type != 'MESH':
				continue

			self.removeUntextured(obj)

		self.session.selectObj(None)

	def removeUntextured(self, obj):
		self.session.selectObj(obj);
		doRemove = False

		# Must use FACE selection or polygons cannot be selected
		# Must be in EDIT mode to do this
		# Must change back to OBJECT mode to do the actual selection
		bpy.ops.object.mode_set(mode='EDIT')
		bpy.ops.mesh.select_mode(type="FACE")
		bpy.ops.object.mode_set(mode='OBJECT')

		# Doesn't seem to actually work
		#bpy.ops.object.select_all(action='DESELECT')

		# Manually go through faces and deselect
		for face in obj.data.polygons:
			face.select = False

		for face in obj.data.polygons:
			slot = obj.material_slots[face.material_index]
			mat = slot.material

			if self.isTextured(mat) == False:
				face.select = True
				doRemove = True

		if doRemove == True:
			bpy.ops.object.mode_set(mode='EDIT')
			bpy.ops.mesh.delete(type='FACE')
			bpy.ops.object.mode_set(mode='OBJECT')

		self.session.selectObj(None)

	def isTextured(self, mat):
		if mat == None:
			return False

		if mat.use_nodes == False:
			return False

		foundTex = False

		for node in mat.node_tree.nodes:
			if node.type == 'TEX_IMAGE':
				if node.image == None:
					print("Warning: Face material has multiple textures (removing)")
					return False
				else:
					foundTex = True

		if foundTex == False:
			return False
	
		return True
