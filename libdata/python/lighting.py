def lighting(state):
  for obj in bpy.data.objects:
    if obj.type != 'LAMP':
      continue

    selectObj(obj)
    bpy.ops.object.delete()
    selectObj(None)

  for obj in bpy.data.objects:
    if obj.type != 'MESH':
      continue

    if obj.name.find("Light(") != -1:

      print("**")
      print("** Removing [Light]")
      print("**")

      pos = obj.data.vertices[obj.data.polygons[0].vertices[0]].co
      cpos = (pos[0], -pos[2], pos[1])

      #print(cpos[0])
      #print(cpos[1])
      #print(cpos[2])

      #light = bpy.ops.object.lamp_add(type='POINT', location=obj.location)
      bpy.ops.object.lamp_add(type='POINT', location=cpos)
      light = bpy.context.scene.objects.active
      emissionNode = light.data.node_tree.nodes["Emission"]
      #emissionNode.inputs[1].default_value = 1000
      emissionNode.inputs[1].default_value = 100000

      #emissionNode.inputs[0].default_value = (1.0, 0.1, 0.1, 1)
      #emissionNode.inputs[0].default_value = (1, 0, 0, 1)

      selectObj(obj)
      bpy.ops.object.delete()
      selectObj(None)
