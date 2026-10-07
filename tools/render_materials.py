"""Original physical editor materials. Blender 5.x, Cycles CPU, no downloaded art.
Run: blender --background --factory-startup --python tools/render_materials.py
"""
import bpy
import math
import random
import re
import json
from pathlib import Path
from mathutils import Vector

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'resources' / 'images'
OUT.mkdir(parents=True, exist_ok=True)
random.seed(87)
bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete(use_global=False)
scene = bpy.context.scene
scene.render.engine = 'CYCLES'
scene.cycles.device = 'CPU'
scene.cycles.samples = 40
scene.cycles.use_denoising = True
scene.cycles.max_bounces = 6
scene.render.threads_mode = 'FIXED'
scene.render.threads = 6
scene.render.image_settings.file_format = 'PNG'
scene.render.image_settings.color_mode = 'RGBA'
scene.render.resolution_percentage = 100
scene.view_settings.view_transform = 'AgX'
scene.world.color = (0.15, 0.15, 0.15)
scene.world.use_nodes = True
scene.world.node_tree.nodes['Background'].inputs['Color'].default_value = (0.24, 0.24, 0.24, 1)
scene.world.node_tree.nodes['Background'].inputs['Strength'].default_value = 0.32


def material(name, color, metallic=0, roughness=0.5, worn=False):
    m = bpy.data.materials.new(name)
    m.use_nodes = True
    n, links = m.node_tree.nodes, m.node_tree.links
    p = n.get('Principled BSDF')
    p.inputs['Base Color'].default_value = (*color, 1)
    p.inputs['Metallic'].default_value = metallic
    p.inputs['Roughness'].default_value = roughness
    position = n.new('ShaderNodeNewGeometry')
    tex = n.new('ShaderNodeTexNoise')
    links.new(position.outputs['Position'], tex.inputs['Vector'])
    tex.inputs['Scale'].default_value = 180
    tex.inputs['Detail'].default_value = 3
    bump = n.new('ShaderNodeBump')
    bump.inputs['Strength'].default_value = 0.28
    bump.inputs['Distance'].default_value = 0.007
    links.new(tex.outputs['Fac'], bump.inputs['Height'])
    links.new(bump.outputs['Normal'], p.inputs['Normal'])
    if worn:
        # Fixed world-space grain. Wear itself is placed at seams and bearings below.
        grain = n.new('ShaderNodeTexNoise')
        grain.inputs['Scale'].default_value = 4
        links.new(position.outputs['Position'], grain.inputs['Vector'])
        ramp = n.new('ShaderNodeValToRGB')
        ramp.color_ramp.elements[0].color = (*(c*0.91 for c in color), 1)
        ramp.color_ramp.elements[1].color = (*(c*1.04 for c in color), 1)
        links.new(grain.outputs['Fac'], ramp.inputs[0])
        links.new(ramp.outputs[0], p.inputs['Base Color'])
    return m

metal = material('exposed aluminium', (0.29, 0.31, 0.27), 0.9, 0.32)
black = material('worn charcoal powdercoat', (0.028, 0.029, 0.027), 0.55, 0.42, False)
green = material('chipped green enamel', (0.038, 0.19, 0.025), 0.62, 0.3, True)
rubber = material('rubber gasket', (0.008, 0.009, 0.008), 0.0, 0.77)
red = material('scraped vermilion enamel', (0.36, 0.024, 0.012), 0.45, 0.36, True)


def box(name, x, y, w, h, z, depth, mat, bevel=0.03):
    bpy.ops.mesh.primitive_cube_add(size=1, location=(x, y, z))
    obj = bpy.context.object
    obj.name = name
    obj.dimensions = (w, h, depth)
    bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)
    obj.data.materials.append(mat)
    if bevel:
        mod = obj.modifiers.new('machined edge', 'BEVEL')
        mod.width = bevel
        mod.segments = 4
        obj.modifiers.new('weighted normals', 'WEIGHTED_NORMAL')
    return obj


def pxbox(name, rect, z, depth, mat, bevel=0.03):
    l, t, r, b = rect
    return box(name, ((l+r)/2-640)/100, (400-(t+b)/2)/100, (r-l)/100, (b-t)/100, z, depth, mat, bevel)


def cylinder(name, x, y, radius, depth, z, mat, vertices=96):
    bpy.ops.mesh.primitive_cylinder_add(vertices=vertices, radius=radius, depth=depth, location=(x,y,z))
    obj = bpy.context.object
    obj.name = name
    obj.data.materials.append(mat)
    mod = obj.modifiers.new('rim bevel', 'BEVEL'); mod.width = min(0.015, depth/3); mod.segments = 3
    obj.modifiers.new('weighted normals','WEIGHTED_NORMAL')
    return obj


def screw(x, y, z):
    x, y = (x-640)/100, (400-y)/100
    cylinder('screw seat', x, y, 0.066, 0.016, z, rubber)
    cylinder('slotted fastener', x, y, 0.048, 0.021, z+0.01, metal)
    slot = box('screw slot', x, y, 0.068, 0.012, z+0.022, 0.005, rubber, 0.003)
    slot.rotation_euler.z = random.uniform(-1, 1)


bpy.ops.object.camera_add(location=(0,0,20))
cam = bpy.context.object
cam.rotation_euler = (0,0,0)
cam.data.type = 'ORTHO'; cam.data.ortho_scale = 12.8
scene.camera = cam


def light(name, loc, energy, size, color, target=(0,0,0)):
    data = bpy.data.lights.new(name, 'AREA'); data.energy = energy; data.shape = 'DISK'; data.size = size; data.color = color
    ob = bpy.data.objects.new(name, data); scene.collection.objects.link(ob); ob.location = loc
    ob.rotation_euler = (Vector(target)-ob.location).to_track_quat('-Z','Y').to_euler()
    return ob

light('single upper-left softbox', (-8,10,15), 2500, 8, (1.0,0.95,0.86))
# Uniform world fill is non-directional; every component uses this same scene.

pxbox('roadcase body', (0,0,1280,800), -0.16, 0.32, rubber, 0.12)
pxbox('exposed perimeter', (7,7,1273,793), 0.025, 0.12, metal, 0.06)
pxbox('green enamel chassis', (14,14,1266,786), 0.09, 0.08, green, 0.04)
# A single work area and permanent channel, not repeated dashboard cards.
pxbox('workspace gasket', (29,199,1011,726), 0.15, 0.03, rubber, 0.035)
pxbox('workspace bevel', (33,203,1007,722), 0.17, 0.04, metal, 0.028)
pxbox('workspace', (36,206,1004,719), 0.20, 0.035, black, 0.018)
pxbox('channel gasket', (1023,199,1254,726), 0.15, 0.03, rubber, 0.035)
pxbox('channel bevel', (1027,203,1250,722), 0.17, 0.04, metal, 0.025)
pxbox('channel', (1030,206,1247,719), 0.20, 0.035, black, 0.018)
nameplate=pxbox('red stamped nameplate', (33,32,306,113), 0.218, 0.025, red, 0.012)
nameplate.rotation_euler.z=math.radians(-1.2)
pxbox('preset recess', (329,53,1058,114), 0.16, 0.022, rubber, 0.025)
pxbox('keyboard recess', (375,739,1253,794), 0.13, 0.012, rubber, 0.02)
for i in range(4):
    left=37+i*243
    pxbox('page switch mount', (left,140,left+232,189), 0.17, 0.035, metal, 0.017)
# Fine exposed edge scratches follow the outside seam, leaving labels readable.
for i in range(100):
    x=random.uniform(20,1260)
    y=random.choice([random.uniform(16,23),random.uniform(775,783)])
    scratch=pxbox('edge abrasion', (x,y,x+random.uniform(0.8,7),y+random.uniform(0.2,0.7)),0.133,0.001,metal,0)
    scratch.rotation_euler.z=random.uniform(-0.5,0.5)
for xx,yy in [(24,25),(1254,25),(24,775),(1258,773),(42,37),(295,107),(1018,213),(1018,706)]:
    screw(xx,yy,0.23)

# Native geometry is the authority for the render. There are no resized dial sprites.
layout_text=(ROOT/'src/editor/ControlLayout.h').read_text()
pattern=r'\{(kParam\w+), "(\w+)", ([^}]+)\}'
placements=[]
for parameter,page,values in re.findall(pattern,layout_text):
    values=[float(v.strip().removesuffix('f')) for v in values.split(',')]
    if len(values)!=5:
        raise RuntimeError('Malformed control placement: '+parameter)
    l,t,r,b,maximum=values
    widget_height=b-t-39
    body=min(min(r-l,widget_height)*0.4,maximum)
    placements.append(dict(parameter=parameter,page=page,cx=(l+r)/2,cy=t+widget_height/2,radius=body))
if len(placements)!=27 or len({p['parameter'] for p in placements})!=27:
    raise RuntimeError('Expected 27 unique physical control placements')


def dial_at(item):
    cx,cy,rad=item['cx'],item['cy'],item['radius']
    x,y,r=(cx-640)/100,(400-cy)/100,rad/100
    z=0.24 if item['parameter']!='kParamGain' else 0.15
    height=r*0.38
    cylinder(item['parameter']+' bearing',x,y,r*1.08,0.012,z,rubber)
    cylinder(item['parameter']+' skirt',x,y,r,0.018,z+0.018,metal)
    cylinder(item['parameter']+' grip',x,y,r*0.9,height,z+0.027+height/2,rubber)
    cylinder(item['parameter']+' cap',x,y,r*0.82,0.016,z+0.029+height,black)
    for i in range(28):
        theta=i*math.tau/28
        ob=box('grip ridge',x+math.cos(theta)*r*0.885,y+math.sin(theta)*r*0.885,
               r*0.085,r*0.022,z+0.026+height/2,height*0.84,black,min(0.004,r*0.014))
        ob.rotation_euler.z=theta
    # Sparse concentric wear is local to a touched control's bearing, not a global texture.
    for i in range(7):
        theta=random.uniform(math.pi*0.65,math.pi*1.65)
        w=random.uniform(0.012,0.035)
        ob=box('bearing contact wear',x+math.cos(theta)*r*1.08,y+math.sin(theta)*r*1.08,
               w,0.004,z+0.008,0.001,metal,0)
        ob.rotation_euler.z=theta+math.pi/2


for item in placements:
    if item['page']=='shared': dial_at(item)
scene.render.resolution_x=2560; scene.render.resolution_y=1600
scene.render.filepath=str(OUT/'chassis.png')
bpy.ops.wm.save_as_mainfile(filepath=str(ROOT/'design'/'physical-editor.blend'))
bpy.ops.render.render(write_still=True)
print('ASSET_WRITTEN chassis.png',flush=True)
scene.render.use_border=True
scene.render.use_crop_to_border=True
scene.render.border_min_x=34/1280; scene.render.border_max_x=1004/1280
scene.render.border_min_y=(800-718)/800; scene.render.border_max_y=(800-204)/800
receipts=[]
for page in ['source','pitch','tape']:
    previous=set(scene.objects)
    for item in placements:
        if item['page']==page: dial_at(item)
    # The two oscillator screens are recessed glass; their waveforms stay live.
    if page=='source':
        for y in [224,460]:
            pxbox('oscillator display seal',(242,y+25,723,y+111),0.236,0.006,rubber,0.012)
            pxbox('oscillator display glass',(247,y+29,718,y+107),0.242,0.004,black,0.006)
    scene.render.filepath=str(OUT/('workspace-'+page+'.png'))
    bpy.ops.render.render(write_still=True)
    receipts.append(dict(page=page,bounds=[34,204,1004,718],controls=[p for p in placements if p['page']==page]))
    for obj in set(scene.objects)-previous:
        bpy.data.objects.remove(obj,do_unlink=True)
    print('ASSET_WRITTEN workspace-'+page+'.png',flush=True)
(OUT/'physical-layout.json').write_text(json.dumps(dict(
    camera='orthographic 12.8',resolution=[2560,1600],
    lighting='single upper-left softbox plus nondirectional world fill',
    material_coordinates='Geometry.Position world space',
    shared=[p for p in placements if p['page']=='shared'],pages=receipts),indent=2))
print('MATERIAL_RENDER_COMPLETE Blender',bpy.app.version_string,flush=True)
