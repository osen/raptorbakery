import bpy
import sys

#######################################################################
# State
#######################################################################
class State:
  pass

#######################################################################
# main
#######################################################################
def main():
  print("Hello World")
  state = State()

  setup(state)
  importObj(state)
  lighting(state)
  clean(state)
  bake(state)
  exportObj(state)

#######################################################################
# setup
#
# Initial config of the environment. The biggest change is setting
# the cycles render (instead of Blender Render or Eevee). This changes
# how a lot of properties are handled.
#######################################################################
def setup(state):
  bpy.context.scene.render.engine = 'CYCLES'
  bpy.ops.image.new(name="__White", width=1, height=1, color=(1, 1, 1, 1))
  state.whiteTexture = bpy.data.images['__White']

  #state.lightmapSize = 1024
  state.lightmapSize = 512
  #state.lightmapSize = 64

  #state.lightMapFormat = "jpg"
  state.lightMapFormat = "png"

#######################################################################
# importObj
#######################################################################
def importObj(state):
  print("**")
  print("** Importing")
  print("**")

  args = sys.argv
  found = -1

  for i in range(len(args)):
    if args[i] == "--":
      found = i
      break

  if found == -1:
    raise Exception("No model specified")

  if found + 2 >= len(args):
    raise Exception("Not enough arguments")

  state.path = args[found + 1]
  state.outPath = args[found + 2]

  bpy.ops.import_scene.obj(filepath=state.path)
  selectObj(None)

#######################################################################
# selectObj
#######################################################################
def selectObj(obj):
  #
  # Must be in object mode in order to deselect all objects. This
  # should always be kept as the default mode. Edit should only be
  # a temporary state. Note if no object selected object mode will
  # cause an error when changing mode.
  #
  #bpy.ops.object.mode_set(mode='OBJECT')

  #
  # Deselect all from the multi-select system
  #
  bpy.ops.object.select_all(action='DESELECT')

  #
  # Select in the multi-select system
  #
  if obj != None:
    obj.select = True

  #
  # Select as "last clicked" object
  #
  bpy.context.scene.objects.active = obj

