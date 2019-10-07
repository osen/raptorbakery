import bpy;

class Initial:
	def __init__(self):
		bpy.context.scene.render.engine = 'CYCLES'

ses = Session()
ini = Initial()
imp = Import()
pro = Process(ses)
exp = Export(ses)

