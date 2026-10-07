"""Read-only image lane discovery. Never starts or changes services."""
import datetime
import json
from pathlib import Path
import subprocess
import urllib.request

OUT = Path(__file__).resolve().parent
REMOTE = r'''
import json,pathlib,subprocess,urllib.request
root=pathlib.Path('/home/sss0lace/sam/comfyui')
r={'comfyui_root_exists':root.is_dir(),'venv_python_exists':(root/'.venv/bin/python').exists(),'model_directories':{}}
for name in ['diffusion_models','unet','checkpoints','text_encoders','clip','vae','loras']:
    p=root/'models'/name
    r['model_directories'][name]=[{'name':str(f.relative_to(p)),'bytes':f.stat().st_size} for f in p.rglob('*') if f.is_file() and not f.name.startswith('.') and not f.name.startswith('put_')] if p.exists() else None
for path in ['/system_stats','/object_info/UNETLoader','/object_info/CheckpointLoaderSimple','/object_info/CLIPLoader','/object_info/VAELoader']:
    try:
        with urllib.request.urlopen('http://127.0.0.1:8188'+path,timeout=5) as response:r[path]=json.load(response)
    except Exception as exc:r[path]={'error':str(exc)}
r['gpu']=subprocess.run(['nvidia-smi','--query-gpu=index,name,memory.total,memory.used,memory.free,utilization.gpu','--format=csv,noheader'],capture_output=True,text=True).stdout
r['comfyui_unit_state']=subprocess.run(['systemctl','is-active','ai-chud-comfy.service'],capture_output=True,text=True).stdout.strip()
r['text_unit_state']=subprocess.run(['systemctl','is-active','ai-chud-qwen.service'],capture_output=True,text=True).stdout.strip()
r['process_names']=[line for line in subprocess.run(['ps','-eo','pid,comm'],capture_output=True,text=True).stdout.splitlines() if any(word in line.lower() for word in ['python','llama','comfy','ollama'])]
print(json.dumps(r))
'''
result = {'timestamp_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(), 'host': 'ai-chud', 'mode': 'read-only discovery', 'restrictions': ['no installation', 'no service stop or start', 'no lane changes', 'no paid APIs']}
p = subprocess.run(['ssh', '-o', 'BatchMode=yes', '-o', 'ConnectTimeout=10', 'ai-chud', 'python3 -'], input=REMOTE, text=True, capture_output=True, timeout=60)
result['ssh_returncode'] = p.returncode
if p.returncode == 0:
    result['remote'] = json.loads(p.stdout)
else:
    result['ssh_error'] = p.stderr
try:
    with urllib.request.urlopen('http://100.120.35.107:8188/system_stats', timeout=8) as response:
        result['tailnet_system_stats'] = json.load(response)
except Exception as exc:
    result['tailnet_system_stats'] = {'error': str(exc)}
path = OUT / 'lane-discovery.json'
path.write_text(json.dumps(result, indent=2), encoding='utf-8')
print(json.dumps(result, indent=2))
print('EVIDENCE_PATH=' + str(path))
