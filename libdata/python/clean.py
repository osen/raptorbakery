#######################################################################
# clean
#######################################################################
def clean(state):
  print("**")
  print("** Cleaning")
  print("**")

  for obj in bpy.data.objects:
    if obj.type != 'MESH':
      continue

    removeUntextured(obj)

    # TODO: Remove objects with no faces

#######################################################################
# removeUntextured
#######################################################################
def removeUntextured(obj):
  selectObj(obj)

  #
  # Put Blender into face select mode (rather than vertices). This can only
  # be done in edit mode. Then deselect all faces.
  #
  bpy.ops.object.mode_set(mode='EDIT')
  bpy.ops.mesh.select_mode(type="FACE")
  bpy.ops.mesh.select_all(action='DESELECT')

  #
  # Change to object mode so that individual faces can be selected. This
  # seems a little counterintuitive.
  bpy.ops.object.mode_set(mode='OBJECT')

  doRemove = False

  for face in obj.data.polygons:
    if isFaceTextured(face, obj) == False:
      face.select = True
      doRemove = True

  if doRemove == True:
    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.mesh.delete(type='FACE')
    bpy.ops.object.mode_set(mode='OBJECT')

  selectObj(None)

#######################################################################
# isFaceTextured
#######################################################################
def isFaceTextured(face, obj):
  slot = obj.material_slots[face.material_index]
  mat = slot.material

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

