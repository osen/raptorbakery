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

    name = obj.name
    lstart = name.find("Light(")

    if lstart != -1:

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
      #emissionNode.inputs[1].default_value = 100000

      #emissionNode.inputs[0].default_value = (1.0, 0.1, 0.1, 1)

      lend = -1

      for i in range(len(name)):
        if name[i] == ')':
          lend = i

      if lend == -1:
        raise Exception("Light format invalid")

      vecstr = name[lstart + 6:lend]
      vecstr = vecstr.split(',')
      strength = (float(vecstr[0]) / 100) * state.maxBrightness
      red = float(vecstr[1]) / 100
      green = float(vecstr[2]) / 100
      blue = float(vecstr[3]) / 100

      emissionNode.inputs[1].default_value = strength
      emissionNode.inputs[0].default_value = (red, green, blue, 1)

      selectObj(obj)
      bpy.ops.object.delete()
      selectObj(None)
