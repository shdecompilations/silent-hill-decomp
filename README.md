<picture>
  <source media="(prefers-color-scheme: dark)" srcset="https://github.com/Vatuu/silent-hill-decomp/blob/master/docs/SHDecompLogo.png?raw=true">
  <source media="(prefers-color-scheme: light)" srcset="https://github.com/Vatuu/silent-hill-decomp/blob/master/docs/SHDecompLogo-NTSC.png?raw=true">
  <img alt="SILENT HILL DECOMPILATION PROJECT" title="SILENT HILL DECOMPILATION PROJECT" src="https://github.com/Vatuu/silent-hill-decomp/blob/master/docs/SHDecompLogo-NTSC.png?raw=true">
</picture>
<div align="center">
<br/>
An in-progress decompilation of <i>Silent Hill</i> on the PlayStation 1.
</div>

> [!IMPORTANT]
**PORT CONCERN** — This project itself **is not**, and **will not**, produce a port, to PC or any other platform. This is a **decompilation** of the original PlayStation 1 game code which can be compiled back to a binary identical to the original PS1 release. This codebase is **not** port-ready due to missing documentation and constant reorganization. This is a hobby project led by volunteered hands, so there is no rush.

> [!IMPORTANT]
**AI CONCERN** — This project is human-driven. **No AI-generated code has knowingly been added into the decompilation**, instead being manually worked out by several contributors over the past few years.
At most some throwaway decompilation-helper scripts were made with AI, but the actual game code is being studied and reverse-engineered by humans.
While forks may use AI for ports/fixes/adding features, **they are NOT endorsed whatsoever**. 

## Progress

### *What does the decompilation percentage mean?*

The percentage tracks how much of the game's compiled code has been matched, meaning we've written C code that compiles to an output identical to the original assembly code.

Reaching 100% means every function in the game is accounted for, but that's **only the end of the first phase, not the project as a whole**.

There's still a lot of work ahead:

- **Deobfuscation and naming**: many functions and variables still have generated names like `func_80241A30`. We'll need to figure out the actual purposes of these and name them appropriately.
- **Data migration**: raw binary data needs to be parsed into proper C structs so the data can be made understandable.
- **Shiftability**: making the build not rely on hardcoded memory addresses so that code and data can be modified without breaking everything.
- **Documentation**: understanding and documenting how the game's systems work together to aid in mods and future projects.

100% is a milestone worth celebrating, but there's still plenty left to do!

### *What are the multiple progress trackers for?*

Due to the limited memory on the PlayStation 1, games often distribute their logic and functionality across different binary overlays. *Silent Hill* follows this approach by separating core engine code, some screen-related code, and map stage logic code into many distinct binaries. The main executable (`SLUS_007.07` on the 1.1 NTSC release) serves primarily as a memory handler.

<table align=center>
    <tbody>
        <tr>
            <th colspan=3>Total Progress</th>
        </tr>
        <tr>
            <th>Version</th>
            <th>Code</th>
            <th>Data</th>
        </tr>
        <tr>
            <td>USA (SLUS-00707)</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&measure=matched_code_percent&label=Code&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&measure=matched_data_percent&label=Data"/></a></td>
        </tr>
        <tr>
            <td>JPN (SLPM-86192)</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&measure=matched_code_percent&label=Code"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&measure=matched_data_percent&label=Data"/></a></td>
        </tr>
        <tr>
            <td>EUR (SLES-01514)</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&measure=matched_code_percent&label=Code"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&measure=matched_data_percent&label=Data"/></a></td>
        </tr>
    </tbody>
</table>

<table align=center>
    <tbody>
        <tr>
            <th colspan=5>⚙ Executables ⚙</th>
        </tr>
        <tr>
            <th>Binary</th>
            <th>USA</th>
            <th>JPN</th>
            <th>EUR</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td><code>SLUS-00707</code> / <code>SLPM-86192</code> / <code>SLES-01514</code></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=main"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=main&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=main"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=main&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=main"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=main&measure=fuzzy_match_percent&label=EUR&color=rgb(255,215,0)"/></a></td>
            <td>Main executable.</td>
        </tr>
        <tr>
            <td>🧟‍♂️ BODYPROG.BIN</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=engine"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=engine&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=engine"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=engine&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=engine"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=engine&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Main game logic.</td>
        </tr>
        <tr>
            <th colspan=5>🎮 Game Screen Overlays 🎮</th>
        </tr>
        <tr>
            <th>Overlay</th>
            <th>USA</th>
            <th>JPN</th>
            <th>EUR</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td>All screens</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=screen"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=screen&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=screen"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=screen&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=screen"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=screen&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Unique game screens.</td>
        </tr>
<tr>
    <td colspan=5 align=center>
<details>
<summary>Click here to expand</summary>
<!-- Github incorrectly parses it if it's indented... -->
<table>
    <tbody>
        <tr>
            <th>Overlay</th>
            <th>USA</th>
            <th>JPN</th>
            <th>EUR</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td>👨‍💼 B_KONAMI.BIN</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=screen.b_konami"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=screen.b_konami&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=screen.b_konami"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=screen.b_konami&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=screen.b_konami"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=screen.b_konami&measure=fuzzy_match_percent&label=EUR&color=rgb(255,215,0)"/></a></td>
            <td>Boot screen logic.<br/><sub><a href="https://github.com/Vatuu/silent-hill-decomp/commit/349b284d61a6901df3b96b99a612a66653d97238">First fully decompiled and matching overlay!</a></sub></td>
        </tr>
        <tr>
            <td>🎞 STREAM.BIN</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=screen.stream"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=screen.stream&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=screen.stream"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=screen.stream&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=screen.stream"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=screen.stream&measure=fuzzy_match_percent&label=EUR&color=rgb(255,215,0)"/></a></td>
            <td>Full motion videos stream logic.<br/><sub><a href="https://github.com/Vatuu/silent-hill-decomp/commit/d2541f2c10667860048a6bd17bc39bbd87280d47">Third fully decompiled and matching overlay!</a></sub></td>
        </tr>
        <tr>
            <td>💾 SAVELOAD.BIN</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=screen.saveload"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=screen.saveload&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=screen.saveload"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=screen.saveload&measure=fuzzy_match_percent&label=JAP0"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=screen.saveload"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=screen.saveload&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Save and load screen logic.<br/><sub><a href="https://github.com/Vatuu/silent-hill-decomp/commit/f72646453302f8c84933decf1326c9b9ebd3d622">Fourth fully decompiled and matching overlay!</a></sub></td>
        </tr>
        <tr>
            <td>📜 STF_ROLL.BIN</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=screen.stf_roll"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=screen.stf_roll&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=screen.stf_roll"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=screen.stf_roll&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=screen.stf_roll"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=screen.stf_roll&measure=fuzzy_match_percent&label=EUR&color=rgb(255,215,0)"/></a></td>
            <td>Credits roll logic.<br/><sub><a href="https://github.com/Vatuu/silent-hill-decomp/commit/701c1223daa79fa6317ee4d09d54ff3d99112abc">Second fully decompiled and matching overlay!</a></sub></td>
        </tr>
        <tr>
            <td>🛠 OPTION.BIN</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=screen.option"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=screen.option&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=screen.option"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=screen.option&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=screen.option"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=screen.option&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Options screen logic.<br/><sub><a href="https://github.com/Vatuu/silent-hill-decomp/commit/a7ab294077dab8104a18ddacb8e567c23eaf0efe">Fifth fully decompiled and matching overlay!</a></sub></td>
        </tr>
    </tbody>
</table>
</details>
    </td>
</tr>
        <tr>
            <th colspan=5>🗺 Game Map Overlays 🗺</th>
        </tr>
        <tr>
            <th>Overlay</th>
            <th>USA</th>
            <th>JPN</th>
            <th>EUR</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td>All maps</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Map and character code.</td>
        </tr>
<tr>
    <td colspan=5 align=center>
<details>
<summary>Click here to expand</summary>
<!-- Github incorrectly parses it if it's indented... -->
<table>
    <tbody>
        <tr>
            <th colspan=5>🏙 MAP0 🏙</th>
        </tr>
        <tr>
            <th>Overlay</th>
            <th>USA</th>
            <th>JPN</th>
            <th>EUR</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td>MAP0_S00</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map0_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map0_s00&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map0_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map0_s00&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map0_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map0_s00&measure=fuzzy_match_percent&label=EUR&color=rgb(255,215,0)"/></a></td>
            <td>Intro sequence in Old Silent Hill.</td>
        </tr>
        <tr>
            <td>MAP0_S01</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map0_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map0_s01&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map0_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map0_s01&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map0_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map0_s01&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Cafe in Old Silent Hill.</td>
        </tr>
        <tr>
            <td>MAP0_S02</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map0_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map0_s02&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map0_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map0_s02&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map0_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map0_s02&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Bonus unlockable areas in Old Silent Hill.</td>
        </tr>
        <tr>
            <th colspan=5>🏫 MAP1 🏫</th>
        </tr>
        <tr>
            <th>Overlay</th>
            <th>USA</th>
            <th>JPN</th>
            <th>EUR</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td>MAP1_S00</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map1_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map1_s00&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map1_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map1_s00&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map1_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map1_s00&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>School 1st floor, courtyard, and basement.</td>
        </tr>
        <tr>
            <td>MAP1_S01</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map1_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map1_s01&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map1_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map1_s01&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map1_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map1_s01&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>School 2nd floor.</td>
        </tr>
        <tr>
            <td>MAP1_S02</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map1_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map1_s02&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map1_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map1_s02&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map1_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map1_s02&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>School 1st floor and courtyard in Otherworld.</td>
        </tr>
        <tr>
            <td>MAP1_S03</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map1_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map1_s03&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map1_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map1_s03&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map1_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map1_s03&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>School 2nd floor and roof in Otherworld.</td>
        </tr>
        <tr>
            <td>MAP1_S04</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map1_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map1_s04&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map1_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map1_s04&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map1_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map1_s04&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Unused <a href="https://github.com/Vatuu/silent-hill-decomp/issues/335#issuecomment-3393749791">(Click here for more information)</a><br/><sub><a href="https://github.com/Vatuu/silent-hill-decomp/commit/c24f6f392e14eb4dbeac7fb0f0975f91a6539cb4">First fully decompiled and matching map overlay!</a></sub></td>
        </tr>
        <tr>
            <td>MAP1_S05</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map1_s05"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map1_s05&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map1_s05"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map1_s05&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map1_s05"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map1_s05&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>School boss fight.</td>
        </tr>
        <tr>
            <td>MAP1_S06</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map1_s06"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map1_s06&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map1_s06"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map1_s06&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map1_s06"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map1_s06&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>School 1st floor & basement after boss fight.</td>
        </tr>
        <tr>
            <th colspan=5>🏙 MAP2 🏙</th>
        </tr>
        <tr>
            <th>Overlay</th>
            <th>USA</th>
            <th>JPN</th>
            <th>EUR</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td>MAP2_S00</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map2_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map2_s00&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map2_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map2_s00&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map2_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map2_s00&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Old Silent Hill.</td>
        </tr>
        <tr>
            <td>MAP2_S01</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map2_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map2_s01&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map2_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map2_s01&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map2_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map2_s01&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Church.<br/><sub><a href="https://github.com/Vatuu/silent-hill-decomp/commit/5921aa73fb75645a75ae9c6fa0fdc30a0e4d58b2">Third fully decompiled and matching map overlay!</a></sub></td>
        </tr>
        <tr>
            <td>MAP2_S02</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map2_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map2_s02&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map2_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map2_s02&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map2_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map2_s02&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Central Silent Hill.</td>
        </tr>
        <tr>
            <td>MAP2_S03</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map2_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map2_s03&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map2_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map2_s03&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map2_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map2_s03&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Unused <a href="https://github.com/Vatuu/silent-hill-decomp/issues/335#issuecomment-3393749791">(Click here for more information)</a><br/><sub><a href="https://github.com/Vatuu/silent-hill-decomp/commit/bce45a64f45e2b561b4f0e675a7bda9bfe805dbf">Second fully decompiled and matching map overlay!</a></sub></td>
        </tr>
        <tr>
            <td>MAP2_S04</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map2_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map2_s04&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map2_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map2_s04&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map2_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map2_s04&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Police station in Central Silent Hill.<br/><sub><a href="https://github.com/Vatuu/silent-hill-decomp/commit/d7d655dfb59dc39328e7b14cae290b30ac0e992b">Fourth fully decompiled and matching map overlay!</a></sub></td>
        </tr>
        <tr>
            <th colspan=5>🏥 MAP3 🏥</th>
        </tr>
        <tr>
            <th>Overlay</th>
            <th>USA</th>
            <th>JPN</th>
            <th>EUR</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td>MAP3_S00</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map3_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map3_s00&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map3_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map3_s00&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map3_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map3_s00&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Hospital beginning. Ends once met Kaufmann.</td>
        </tr>
        <tr>
            <td>MAP3_S01</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map3_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map3_s01&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map3_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map3_s01&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map3_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map3_s01&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Hospital 1st and basement floors.</td>
        </tr>
        <tr>
            <td>MAP3_S02</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map3_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map3_s02&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map3_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map3_s02&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map3_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map3_s02&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Antique Shop cutscene in the Hospital.</td>
        </tr>
        <tr>
            <td>MAP3_S03</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map3_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map3_s03&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map3_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map3_s03&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map3_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map3_s03&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Hospital 3rd and 2nd floor in Otherworld.</td>
        </tr>
        <tr>
            <td>MAP3_S04</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map3_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map3_s04&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map3_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map3_s04&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map3_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map3_s04&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Hospital 1st floor in Otherworld.</td>
        </tr>
        <tr>
            <td>MAP3_S05</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map3_s05"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map3_s05&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map3_s05"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map3_s05&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map3_s05"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map3_s05&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Hospital basement in Otherworld.</td>
        </tr>
        <tr>
            <td>MAP3_S06</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map3_s06"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map3_s06&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map3_s06"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map3_s06&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map3_s06"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map3_s06&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Hospital 1st floor after Otherworld section.</td>
        </tr>
        <tr>
            <th colspan=5>🏙 MAP4 🏙</th>
        </tr>
        <tr>
            <th>Overlay</th>
            <th>USA</th>
            <th>JPN</th>
            <th>EUR</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td>MAP4_S00</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map4_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map4_s00&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map4_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map4_s00&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map4_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map4_s00&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Unused <a href="https://github.com/Vatuu/silent-hill-decomp/issues/335#issuecomment-3393749791">(Click here for more information)</a></td>
        </tr>
        <tr>
            <td>MAP4_S01</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map4_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map4_s01&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map4_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map4_s01&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map4_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map4_s01&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Antique Shop in Central and Otherworld.</td>
        </tr>
        <tr>
            <td>MAP4_S02</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map4_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map4_s02&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map4_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map4_s02&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map4_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map4_s02&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Central Silent Hill in Otherworld.</td>
        </tr>
        <tr>
            <td>MAP4_S03</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map4_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map4_s03&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map4_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map4_s03&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map4_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map4_s03&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Mall and boss fight.</td>
        </tr>
        <tr>
            <td>MAP4_S04</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map4_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map4_s04&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map4_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map4_s04&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map4_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map4_s04&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Hospital 1st floor.</td>
        </tr>
        <tr>
            <td>MAP4_S05</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map4_s05"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map4_s05&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map4_s05"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map4_s05&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map4_s05"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map4_s05&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Floatstinger boss fight in Otherworld.</td>
        </tr>
        <tr>
            <td>MAP4_S06</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map4_s06"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map4_s06&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map4_s06"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map4_s06&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map4_s06"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map4_s06&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Unused <a href="https://github.com/Vatuu/silent-hill-decomp/issues/335#issuecomment-3393749791">(Click here for more information)</a></td>
        </tr>
        <tr>
            <th colspan=5>🏙🍹 MAP5 🍹🏙</th>
        </tr>
        <tr>
            <th>Overlay</th>
            <th>USA</th>
            <th>JPN</th>
            <th>EUR</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td>MAP5_S00</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map5_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map5_s00&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map5_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map5_s00&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map5_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map5_s00&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Sewers lower and upper levels.</td>
        </tr>
        <tr>
            <td>MAP5_S01</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map5_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map5_s01&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map5_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map5_s01&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map5_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map5_s01&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Silent Hill Resort Area.</td>
        </tr>
        <tr>
            <td>MAP5_S02</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map5_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map5_s02&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map5_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map5_s02&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map5_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map5_s02&measure=fuzzy_match_percent&label=EUR&color=rgb(255,215,0)"/></a></td>
            <td>Annie's Bar & Indian Runner in Resort Area.</td>
        </tr>
        <tr>
            <td>MAP5_S03</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map5_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map5_s03&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map5_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map5_s03&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map5_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map5_s03&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Norman's Motel in Resort Area.</td>
        </tr>
        <tr>
            <th colspan=5>🏙🍹🔥 MAP6 🔥🍹🏙</th>
        </tr>
        <tr>
            <th>Overlay</th>
            <th>USA</th>
            <th>JPN</th>
            <th>EUR</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td>MAP6_S00</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map6_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map6_s00&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map6_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map6_s00&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map6_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map6_s00&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Resort Area in Otherworld.</td>
        </tr>
        <tr>
            <td>MAP6_S01</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map6_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map6_s01&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map6_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map6_s01&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map6_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map6_s01&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Boat at Lakeside Pier.</td>
        </tr>
        <tr>
            <td>MAP6_S02</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map6_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map6_s02&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map6_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map6_s02&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map6_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map6_s02&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Lakeside Pier and Lighthouse.</td>
        </tr>
        <tr>
            <td>MAP6_S03</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map6_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map6_s03&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map6_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map6_s03&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map6_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map6_s03&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Sewer connecting to Amusement Park.</td>
        </tr>
        <tr>
            <td>MAP6_S04</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map6_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map6_s04&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map6_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map6_s04&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map6_s04"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map6_s04&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Cybil fight and Alessa kidnapping cutscene.</td>
        </tr>
        <tr>
            <td>MAP6_S05</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map6_s05"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map6_s05&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map6_s05"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map6_s05&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map6_s05"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map6_s05&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Unused <a href="https://github.com/Vatuu/silent-hill-decomp/issues/335#issuecomment-3393749791">(Click here for more information)</a><br/><sub><a href="https://github.com/Vatuu/silent-hill-decomp/commit/376dc449e5834bddd93a915529a59fcac7bda6b6">Fifth fully decompiled and matching map overlay!</a></sub></td>
        </tr>
        <tr>
            <th colspan=5>❔🏥 MAP7 🏥❔</th>
        </tr>
        <tr>
            <th>Overlay</th>
            <th>USA</th>
            <th>JPN</th>
            <th>EUR</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td>MAP7_S00</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map7_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map7_s00&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map7_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map7_s00&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map7_s00"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map7_s00&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Hospital 1st floor in Nowhere.<br/><sub>Contains the cutscene with Lisa.</sub></td>
        </tr>
        <tr>
            <td>MAP7_S01</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map7_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map7_s01&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map7_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map7_s01&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map7_s01"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map7_s01&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Unknown parts of Nowhere.</td>
        </tr>
        <tr>
            <td>MAP7_S02</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map7_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map7_s02&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map7_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map7_s02&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map7_s02"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map7_s02&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Unknown parts of Nowhere.<br/><sub>Contains cutscene where Alessa struggles with Dahlia.</sub></td>
        </tr>
        <tr>
            <td>MAP7_S03</td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707?category=map.map7_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLUS-00707.svg?mode=shield&category=map.map7_s03&measure=fuzzy_match_percent&label=USA&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192?category=map.map7_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLPM-86192.svg?mode=shield&category=map.map7_s03&measure=fuzzy_match_percent&label=JAP0&color=rgb(255,215,0)"/></a></td>
            <td align=center><a href="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514?category=map.map7_s03"><img src="https://decomp.dev/shdecompilations/silent-hill-decomp/SLES-01514.svg?mode=shield&category=map.map7_s03&measure=fuzzy_match_percent&label=EUR"/></a></td>
            <td>Final boss fight.</td>
        </tr>
    </tbody>
</table>
</details>
    </td>
</tr>
    </tbody>
</table>

<div align="center">
Progress bars powered by <a href="https://decomp.dev">decomp.dev</a>
</div>

## Contributing
Contributions are welcome! Feel free to open a pull request. To help familiarize yourself with the setup and decompilation workflow for *Silent Hill*, refer to our [Wiki Page](/../../wiki/Home).

You can also reach out to us by opening an issue or joining the `#silent-hill` channel on the [PS1/PS2 Re-Arc (Reverse Engineering, Archaeology, and Reconstruction of Code)](https://discord.gg/SCY699rts) Discord server.
