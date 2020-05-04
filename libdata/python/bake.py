#######################################################################
# ShadowGroup
#######################################################################
class ShadowGroup:
  def __init__(self):
    self.obj = None
    self.texture = None
    self.materialGroups = []

#######################################################################
# MaterialGroup
#######################################################################
class MaterialGroup:
  def __init__(self):
    self.texture = None
    self.mat = None
    self.faces = []

#######################################################################
# SimpleFace
#######################################################################
class SimpleFace:
  def __init__(self):
    self.quad = False

#######################################################################
# bake
#######################################################################
def bake(state):
  print("**")
  print("** Baking")
  print("**")
  state.shadowGroups = []

  for obj in bpy.data.objects:
    if obj.type != 'MESH':
      continue

    generateLightmap(state, obj)

  for obj in bpy.data.objects:
    if obj.type != 'MESH':
      continue

    bakeObj(obj)

#######################################################################
# bakeObj
#######################################################################
def bakeObj(obj):
  if obj.data.uv_layers.active == None:
    print("Ignoring Mesh: " + obj.name)
    return

  selectObj(obj)

  #bpy.context.scene.cycles.samples = 24
  #bpy.context.scene.cycles.use_square_samples = True
  #bpy.ops.object.bake(type='SHADOW')
  #bpy.ops.object.bake(type='COMBINED')
  #bpy.context.scene.cycles.use_pass_indirect = False

  #bpy.ops.object.bake(type='DIFFUSE', pass_filter={'DIRECT'})
  bpy.ops.object.bake(type='COMBINED', pass_filter={'DIRECT', 'DIFFUSE'})

  selectObj(None)

#######################################################################
# generateLightmap
#
# Generate a new shadow group for the specified object. Create the
# required lightmap in Blender and the required UV layer.
#######################################################################
def generateLightmap(state, obj):
  if len(obj.data.uv_layers) < 1:
    print("Ignoring Mesh (No UV layers): " + obj.name)
    return

  if obj.data.uv_layers.active == None:
    print("Ignoring Mesh (UV Layer in invalid state): " + obj.name)
    return

  tcuv = obj.data.uv_layers.active
  lmuv = obj.data.uv_textures.new(name="LightMap")
  obj.data.uv_textures.active = lmuv
  lmuv = obj.data.uv_layers[len(obj.data.uv_layers) - 1]

  selectObj(obj)
  bpy.ops.uv.lightmap_pack(PREF_CONTEXT='ALL_FACES', PREF_PACK_IN_ONE=False)
  selectObj(None)

  lightMap = bpy.data.images.new("LightMap_" + obj.name,
    width=state.lightmapSize, height=state.lightmapSize)

  sg = ShadowGroup()
  sg.texture = lightMap
  sg.obj = obj
  state.shadowGroups.append(sg)

  for m in obj.material_slots:
    prepareMaterial(state, m.material, sg)

  for face in obj.data.polygons:
    prepareFace(state, face, sg, tcuv.data, lmuv.data)

#######################################################################
# prepareFace
#
# Obtain the material group from within the shadow group that matches
# the face material. Copy the face data into it.
#######################################################################
def prepareFace(state, face, sg, uvdat, lmdat):
  mg = None
  mat = sg.obj.material_slots[face.material_index].material

  for m in sg.materialGroups:
    if m.mat == mat:
      mg = m

  if mg == None:
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

  #
  # For hard edges
  #
  #sf.na = face.normal
  #sf.nb = face.normal
  #sf.nc = face.normal

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

#######################################################################
# prepareMaterial
#######################################################################
def prepareMaterial(state, mat, sg):
  print("Preparing Material [" + mat.name + "] in " + sg.obj.name)

  if mat.use_nodes == False:
    mat.use_nodes = True

  mg = MaterialGroup()
  mg.mat = mat
  nodes = mat.node_tree.nodes

  #
  # Replace any texture with a white one. It provides a better shadow map
  #
  for node in nodes:
    if node.type == 'TEX_IMAGE':
      mg.texture = node.image
      node.image = state.whiteTexture
      break

  if mg.texture != None:
    sg.materialGroups.append(mg)

  #
  # Set the render target to the groups lightmap image.
  #
  tiNode = nodes.new('ShaderNodeTexImage')
  tiNode.image = sg.texture
  mat.node_tree.nodes.active = tiNode

