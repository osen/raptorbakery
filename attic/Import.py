import sys;
import bpy;

class Import:
	path = ""

	def __init__(self):
		self.findPath()

		bpy.ops.import_scene.obj(filepath=self.path)

		bpy.ops.object.select_all(action='DESELECT')

	def findPath(self):
		args = sys.argv
		found = -1

		for i in range(len(args)):
			if args[i] == "--":
				found = i
				break

		if found == -1:
			raise Exception("No model specified")

		if found + 1 >= len(args):
			raise Exception("Not enough arguments")

		self.path = args[found + 1]

