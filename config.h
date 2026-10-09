#define PLUG_NAME "MAGS SYNTH"
#define PLUG_MFR "Mags"
#define PLUG_VERSION_HEX 0x00000100
#define PLUG_VERSION_STR "0.1.0"
#define PLUG_UNIQUE_ID 'MgS1'
#define PLUG_MFR_ID 'Mags'
#define PLUG_URL_STR ""
#define PLUG_EMAIL_STR ""
#define PLUG_COPYRIGHT_STR "Private handover instrument"
#define PLUG_CLASS_NAME Keith

// Registered identity is fixed as of 2026-10-08 (subtype MgS1, maker Mags).
// Changing PLUG_UNIQUE_ID or PLUG_MFR_ID later renames the plugin a host already
// stored in a saved set, and those sets stop finding it. Treat both as frozen.
#define BUNDLE_NAME "MagsSynth"
#define BUNDLE_MFR "Mags"
#define BUNDLE_DOMAIN "audio"

#define SHARED_RESOURCES_SUBPATH "MagsSynth"

#define PLUG_CHANNEL_IO "0-2"

#define PLUG_LATENCY 0
#define PLUG_TYPE 1
#define PLUG_DOES_MIDI_IN 1
#define PLUG_DOES_MIDI_OUT 0
#define PLUG_DOES_MPE 0
#define PLUG_DOES_STATE_CHUNKS 1
#define PLUG_HAS_UI 1
// Editor design size. Minimum practical surface 1024x700; the shell uses the Scale
// corner resizer, so smaller windows shrink the whole design uniformly instead of
// clipping or reflowing it. The ceiling is 3x the design size, which is also the
// render scale of every baked face, so a raster face is never upscaled (see
// tools/verify_face_scale.py).
#define PLUG_WIDTH 1280
#define PLUG_HEIGHT 800
#define PLUG_MIN_WIDTH 1024
#define PLUG_MIN_HEIGHT 700
#define PLUG_MAX_WIDTH 3840
#define PLUG_MAX_HEIGHT 2400
#define PLUG_FPS 60
#define PLUG_SHARED_RESOURCES 0
#define PLUG_HOST_RESIZE 0

// Editor faces. OFL 1.1 fonts embedded via resources/main.rc (standalone) and copied
// into the bundle on macOS; see README licence notes.
#define CHASSIS_FN "chassis.png"
#define BANGERS_FN "Bangers-Regular.ttf"
#define OSWALD_FN "Oswald-Bold.ttf"
#define SPACEMONO_FN "SpaceMono-Regular.ttf"
#define SPACEMONO_BOLD_FN "SpaceMono-Bold.ttf"

#define AUV2_ENTRY Keith_Entry
#define AUV2_ENTRY_STR "Keith_Entry"
#define AUV2_FACTORY Keith_Factory
#define AUV2_VIEW_CLASS Keith_View
#define AUV2_VIEW_CLASS_STR "Keith_View"

#define AAX_TYPE_IDS 'MgS1', 'MgS2'
#define AAX_PLUG_MFR_STR "Mags"
#define AAX_PLUG_NAME_STR "MAGS SYNTH\nMgS1"
#define AAX_DOES_AUDIOSUITE 0
#define AAX_PLUG_CATEGORY_STR "Synth"

#define VST3_SUBCATEGORY "Instrument|Synth"

#define APP_NUM_CHANNELS 2
#define APP_N_VECTOR_WAIT 0
#define APP_MULT 1
#define APP_COPY_AUV3 0
#define APP_SIGNAL_VECTOR_SIZE 64
