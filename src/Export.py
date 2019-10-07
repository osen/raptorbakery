import bpy

from shutil import copyfile

class Export:
	session = None
	positions = None
	texcoords = None
	lmcoords = None

	def __init__(self, session):
		self.session = session
		self.positions = []
		self.texcoords = []
		self.lmcoords = []

		self.materials()

		f = open(self.session.outdir + "/" + self.session.outname + ".obj", "w")
		f.write("# Raptor Bakery OBJ File\n\n")

		self.prepare(f)
		self.process(f)

		f.close()

	def materials(self):
		f = open(self.session.outdir + "/" + self.session.outname + ".mtl", "w")

		f.write("# Raptor Bakery MTL File\n\n")

		for sg in self.session.shadowGroups:

			sg.texture.filepath_raw = self.session.outdir + "/" + sg.obj.name + "_lightmap." + self.session.lightMapFormat
			sg.texture.file_format = 'PNG'
			sg.texture.save()

			for mg in sg.materialGroups:
				copyfile(mg.texture.filepath, self.session.outdir + "/" + mg.texture.name)

				f.write("newmtl " + sg.obj.name + "_" + mg.texture.name + "\n")
				f.write("map_Kd " + mg.texture.name + "\n")
				f.write("map_Kl " + sg.obj.name + "_lightmap." + self.session.lightMapFormat + "\n")
				f.write("\n")

		f.close()

	def genindex(self, store, vec):
		for i in range(len(store)):
			if len(vec) == 3:
				if vec[0] == store[i][0] and vec[1] == store[i][1] and vec[2] == store[i][2]:
					return i + 1
			else:
				if vec[0] == store[i][0] and vec[1] == store[i][1]:
					return i + 1

		store.append(vec)
		return len(store)

	def format(self, value):
		return "%.4f" % value

	def process(self, f):
		for sg in self.session.shadowGroups:
			f.write("o " + sg.obj.name + "\n")

			for mg in sg.materialGroups:
				f.write("usemtl " + sg.obj.name + "_" + mg.texture.name + "\n")

				for face in mg.faces:
					f.write("f ")
					f.write(str(self.genindex(self.positions, face.pa)))
					f.write("/")
					f.write(str(self.genindex(self.texcoords, face.ta)))
					f.write("/")
					f.write("/")
					f.write(str(self.genindex(self.lmcoords, face.la)))
					f.write(" ")
					f.write(str(self.genindex(self.positions, face.pb)))
					f.write("/")
					f.write(str(self.genindex(self.texcoords, face.tb)))
					f.write("/")
					f.write("/")
					f.write(str(self.genindex(self.lmcoords, face.lb)))
					f.write(" ")
					f.write(str(self.genindex(self.positions, face.pc)))
					f.write("/")
					f.write(str(self.genindex(self.texcoords, face.tc)))
					f.write("/")
					f.write("/")
					f.write(str(self.genindex(self.lmcoords, face.lc)))

					if face.quad == True:
						f.write(" ")
						f.write(str(self.genindex(self.positions, face.pd)))
						f.write("/")
						f.write(str(self.genindex(self.texcoords, face.td)))
						f.write("/")
						f.write("/")
						f.write(str(self.genindex(self.lmcoords, face.ld)))

					f.write("\n")

	def prepare(self, f):
		for sg in self.session.shadowGroups:
			for mg in sg.materialGroups:
				for face in mg.faces:
					self.genindex(self.positions, face.pa)
					self.genindex(self.positions, face.pb)
					self.genindex(self.positions, face.pc)

					self.genindex(self.texcoords, face.ta)
					self.genindex(self.texcoords, face.tb)
					self.genindex(self.texcoords, face.tc)

					self.genindex(self.lmcoords, face.la)
					self.genindex(self.lmcoords, face.lb)
					self.genindex(self.lmcoords, face.lc)

					if face.quad == True:
						self.genindex(self.positions, face.pd)
						self.genindex(self.texcoords, face.td)
						self.genindex(self.lmcoords, face.ld)

		for pos in self.positions:
			f.write("v ")
			f.write(self.format(pos[0]))
			f.write(" ")
			f.write(self.format(pos[1]))
			f.write(" ")
			f.write(self.format(pos[2]))
			f.write("\n")

		for coord in self.texcoords:
			f.write("vt ")
			f.write(self.format(coord[0]))
			f.write(" ")
			f.write(self.format(coord[1]))
			f.write("\n")

		for coord in self.lmcoords:
			f.write("vl ")
			f.write(self.format(coord[0]))
			f.write(" ")
			f.write(self.format(coord[1]))
			f.write("\n")

