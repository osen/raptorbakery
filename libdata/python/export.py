from shutil import copyfile

def exportObj(state):
  print("**")
  print("** Exporting [" + state.outPath + "]")
  print("**")

  state.positions = []
  state.texcoords = []
  state.normals = []
  state.lmcoords = []

  last = state.outPath.rfind('/')

  if last == -1:
    state.outDir = ""
    tmp = state.outName = state.outPath
  else:
    state.outDir = state.outPath[0:last]
    tmp = state.outName = state.outPath[last + 1:]

  last = state.outName.rfind('.')

  if last == -1:
    raise Exception("Invalid output path specified [" + state.outName + "]")

  state.outName = state.outName[0:last]

  #print(state.outPath)
  #print(state.outDir)
  #print(state.outName)

  exportMaterials(state)

  f = open(state.outDir + "/" + state.outName + ".obj", "w")
  f.write("# Raptor Bakery OBJ File\n\n")
  f.write("mtllib " + state.outName + ".mtl\n\n")

  outputVerts(state, f)
  outputFaces(state, f)

  f.close()

def outputFaces(state, f):
  for sg in state.shadowGroups:
    f.write("o " + sg.obj.name + "\n")

    for mg in sg.materialGroups:
      if len(mg.faces) < 1:
        continue;

      f.write("usemtl " + sg.obj.name + "_" + mg.texture.name + "\n")

      for face in mg.faces:
        f.write("f ")
        f.write(str(genindex(state.positions, face.pa)))
        f.write("/")
        f.write(str(genindex(state.texcoords, face.ta)))
        f.write("/")
        f.write(str(genindex(state.normals, face.na)))
        f.write("/")
        f.write(str(genindex(state.lmcoords, face.la)))
        f.write(" ")
        f.write(str(genindex(state.positions, face.pb)))
        f.write("/")
        f.write(str(genindex(state.texcoords, face.tb)))
        f.write("/")
        f.write(str(genindex(state.normals, face.nb)))
        f.write("/")
        f.write(str(genindex(state.lmcoords, face.lb)))
        f.write(" ")
        f.write(str(genindex(state.positions, face.pc)))
        f.write("/")
        f.write(str(genindex(state.texcoords, face.tc)))
        f.write("/")
        f.write(str(genindex(state.normals, face.nc)))
        f.write("/")
        f.write(str(genindex(state.lmcoords, face.lc)))

        if face.quad == True:
          f.write(" ")
          f.write(str(genindex(state.positions, face.pd)))
          f.write("/")
          f.write(str(genindex(state.texcoords, face.td)))
          f.write("/")
          f.write(str(genindex(state.normals, face.nd)))
          f.write("/")
          f.write(str(genindex(state.lmcoords, face.ld)))

        f.write("\n")

def genindex(store, vec):
  for i in range(len(store)):
    if len(vec) == 3:
      if vec[0] == store[i][0] and vec[1] == store[i][1] and vec[2] == store[i][2]:
        return i + 1
    else:
      if vec[0] == store[i][0] and vec[1] == store[i][1]:
        return i + 1

  store.append(vec)
  return len(store)

def formatVert(value):
  return "%.4f" % value

def outputVerts(state, f):
  for sg in state.shadowGroups:
    for mg in sg.materialGroups:
      for face in mg.faces:
        genindex(state.positions, face.pa)
        genindex(state.positions, face.pb)
        genindex(state.positions, face.pc)

        genindex(state.texcoords, face.ta)
        genindex(state.texcoords, face.tb)
        genindex(state.texcoords, face.tc)

        genindex(state.normals, face.na)
        genindex(state.normals, face.nb)
        genindex(state.normals, face.nc)

        genindex(state.lmcoords, face.la)
        genindex(state.lmcoords, face.lb)
        genindex(state.lmcoords, face.lc)

        if face.quad == True:
          genindex(state.positions, face.pd)
          genindex(state.texcoords, face.td)
          genindex(state.normals, face.nd)
          genindex(state.lmcoords, face.ld)

  for pos in state.positions:
    f.write("v ")
    f.write(formatVert(pos[0]))
    f.write(" ")
    f.write(formatVert(pos[1]))
    f.write(" ")
    f.write(formatVert(pos[2]))
    f.write("\n")

  for coord in state.texcoords:
    f.write("vt ")
    f.write(formatVert(coord[0]))
    f.write(" ")
    f.write(formatVert(coord[1]))
    f.write("\n")

  for norm in state.normals:
    f.write("vn ")
    f.write(formatVert(norm[0]))
    f.write(" ")
    f.write(formatVert(norm[1]))
    f.write(" ")
    f.write(formatVert(norm[2]))
    f.write("\n")

  for coord in state.lmcoords:
    f.write("vl ")
    f.write(formatVert(coord[0]))
    f.write(" ")
    f.write(formatVert(coord[1]))
    f.write("\n")

def exportMaterials(state):
  f = open(state.outDir + "/" + state.outName + ".mtl", "w")

  f.write("# Raptor Bakery MTL File\n\n")

  for sg in state.shadowGroups:
    sg.texture.filepath_raw = state.outDir + "/" + sg.obj.name + "_lightmap." + state.lightMapFormat
    sg.texture.file_format = 'PNG'
    sg.texture.save()

    for mg in sg.materialGroups:
      if len(mg.faces) < 1:
        continue;

      copyfile(mg.texture.filepath, state.outDir + "/" + mg.texture.name)
      f.write("newmtl " + sg.obj.name + "_" + mg.texture.name + "\n")
      f.write("map_Kd " + mg.texture.name + "\n")
      f.write("map_Kl " + sg.obj.name + "_lightmap." + state.lightMapFormat + "\n")
      f.write("\n")

  f.close()

