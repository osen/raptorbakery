class SimpleFace:
	def __init__(self):
		self.pa = None
		self.pb = None
		self.pc = None
		self.pd = None
		self.na = None
		self.nb = None
		self.nc = None
		self.nd = None
		self.normal = None
		self.ta = None
		self.tb = None
		self.tc = None
		self.td = None
		self.la = None
		self.lb = None
		self.lc = None
		self.ld = None
		self.quad = False

class MaterialGroup:
	def __init__(self):
		self.texture = None
		self.mat = None
		self.faces = []

class ShadowGroup:
	def __init__(self):
		self.obj = None
		self.texture = None
		self.materialGroups = []

class Session:
	def __init__(self):
		self.outdir = "tmp"
		self.outname = "sample"
		self.lightMapFormat = "png"
		self.path = "TODO"
		self.version = 280
		self.shadowGroups = []

		ver = bpy.app.version
		self.version = ver[0] * 100 + ver[1]
