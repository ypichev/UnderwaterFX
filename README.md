# 🌊 UnderwaterFX

**UnderwaterFX** is a creative audio plugin (VST3 / Standalone) built with the JUCE framework. It is designed to instantly submerge your audio tracks into deep, lush, and moving aquatic spaces. Perfect for creating lo-fi textures, cinematic transitions, and immersive ambient sound design.

---

## 🚀 Download (For Music Producers)

You don't need to compile anything to use the plugin! The pre-built Windows binaries are ready to go:
👉 **[Download the Latest Release of UnderwaterFX (VST3 & EXE)](https://github.com/ypichev/UnderwaterFX/releases/latest)**

### Installation (Windows):
1. Download and extract the **`UnderwaterFX_Windows.zip`** archive.
2. **VST3 Plugin:** Copy the `UnderwaterFX.vst3` folder and paste it into your system's standard VST3 directory:
   `C:\Program Files\Common Files\VST3`
3. Open your favorite DAW (FL Studio, Ableton Live, Reaper, Cubase, etc.) and perform a plugin rescan.
4. **Standalone App:** Simply run `UnderwaterFX.exe` directly if you want to jam without opening a DAW.

---

## ✨ Key Features & UX

* **Complex DSP Chain:** A custom Low-Pass filter, stereo chorus, and spatial reverberation work simultaneously under the hood.
* **Smart Macro (Main FX):** Dial in the entire underwater effect with a single knob. The higher the value, the deeper the submersion.
* **Manual Mode Toggle:** Flip the switch to unlock individual control over every sub-effect for precise sound design.
* **Premium Animated UI:** Smooth neon glow transitions, dynamic state animations, and responsive real-time stereo VU meters.
* **Advanced Controls:**
  * **Quick Reset:** Double-click any knob to instantly snap it back to its default value.
  * **Precise Typing:** Click the value display box below any slider once to type in your exact numerical values manually.

---

## 🎛 Parameter Breakdown

### Global Controls
* **Main FX** — Macro controller for the overall "submersion" intensity. Enabled by default.
* **Dry / Wet (Mix)** — Standard balance control between the dry input signal and the wet processed effect.
* **Manual Mode** — Toggle switch that disengages the Main FX macro and unlocks individual sub-effect sliders.

### Manual Mode Controls (Sub-Effects)
* **Dive** — Filter cutoff frequency. Lowers the cutoff to muffle high frequencies, simulating the density of deep water.
* **Reso** — Filter resonance. Adds a distinctive synthetic edge at the cutoff point.
* **Chorus** — Chorus modulation depth. Simulates the organic motion of underwater waves and widens the stereo image.
* **Reverb** — Reverb mix level. Shapes the size and wetness of your virtual underwater cave.

---

## 🛠 Build Instructions (For Developers)

If you want to modify the source code or compile the plugin for your own system (e.g., macOS), follow these steps:

### Prerequisites:
* [JUCE Framework](https://juce.com/) (v7.x or newer)
* IDE: Visual Studio 2022 (Windows) or Xcode (macOS)

### Steps:
1. Clone the repository:
   ```bash
   git clone [https://github.com/ypichev/UnderwaterFX.git](https://github.com/ypichev/UnderwaterFX.git)
