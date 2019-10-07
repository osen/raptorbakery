BLENDER=/opt/blender-2.79/blender
#BLENDER=/opt/blender-2.80/blender
SAMPLE=samples/harvest/harvest.obj

all:
	cat src/Import.py > bakery.py
	cat src/Process.py >> bakery.py
	cat src/Session.py >> bakery.py
	cat src/Export.py >> bakery.py
	cat src/Initial.py >> bakery.py

	${BLENDER} --background --python bakery.py -- ${SAMPLE}

