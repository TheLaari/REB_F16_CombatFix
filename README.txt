REB_F16_CombatFix
==================

WHAT'S IN THE ZIP
------------------
@REB_F16_CombatFix/
    mod.cpp
    addons/
        REB_F16_CombatFix/
            $PBOPREFIX$      <- tells Arma this addon's path is "REB_F16_CombatFix"
            config.cpp       <- the actual patch (plain text, unbinarized)

This ships as loose/unpacked files rather than a .pbo. That's intentional:
it's the standard, Bohemia-documented way to run a config change without
needing Arma 3 Tools/Addon Builder installed. It only requires one extra
launch parameter (step 3 below).

INSTALL (vanilla Arma 3 Launcher)
-----------------------------------
1. Unzip so you end up with a folder literally named "@REB_F16_CombatFix"
   containing the "addons" folder and "mod.cpp" directly inside it.

2. In the Launcher, go to MODS, click the folder/"..." icon (or drag-and-
   drop @REB_F16_CombatFix onto the Launcher window) and point it at the
   parent folder containing @REB_F16_CombatFix so it gets added as a
   local mod. Tick it on.

   Order doesn't matter for this patch specifically (the requiredAddons
   entry in config.cpp forces REB_FIR_F16 to load first regardless), but
   it's good practice to keep it below REBalanced - FIR F-16 and AWESome
   in the list.

3. Go to the PARAMETERS tab (or the "..." menu depending on Launcher
   version) and add this to your parameters:

       -filePatching

   This is required because the patch's config.cpp is plain text, not
   compiled into a .pbo. Without it, Arma will ignore the addon.

4. Launch the game, load into the F-16, and test:
     - Does the 1200 km/h "wall" push out further under AB?
     - Does sustained-turn energy bleed feel less punishing?

IMPORTANT LIMITATION
----------------------
-filePatching is fine for single-player, editor, and your own hosted/
local server. Most public multiplayer servers running BattlEye with
signature verification will refuse -filePatching clients or require
signed .pbo mods instead. If you eventually want this for a server:

  1. Install Arma 3 Tools from Steam (free) -> Addon Builder + DSUtils.
  2. Move addons/REB_F16_CombatFix into your P: drive workbench project
     structure the same way any Arma addon source is built.
  3. Run Addon Builder on the REB_F16_CombatFix folder to produce
     REB_F16_CombatFix.pbo.
  4. Generate a keypair with DSUtils/DSCreateKey, sign the .pbo, and
     distribute the .bikey alongside it if it needs to run on a
     signature-checked server.
  5. Drop the resulting REB_F16_CombatFix.pbo into
     @REB_F16_CombatFix/addons/ in place of the loose folder, and you
     can drop -filePatching from your parameters.

TUNING FURTHER
----------------
All three changed values live at the top of config.cpp with the
original REBalanced numbers left in comments right above each one, so
you can nudge them back toward stock or push further:

  - airFrictionCoefs2[]  -> forward-axis quadratic drag term AWESome
                             also uses as its wave-drag base. Lower =
                             less transonic drag.
  - envelope[]            -> lift-coefficient curve vs. speed index.
                             Higher / flatter = less energy bled per
                             degree of AoA in sustained turns.
  - stallSpeed             -> cosmetic/AI-facing; adjust to taste once
                             the two above feel right.

If you change nothing else, this gives you a clean before/after test:
if the transonic wall goes away but turning still feels bad, the drag
fix worked and the lift curve needs more work (or vice versa). If
neither changes much, the bottleneck is inside AWESome's own global
aerodynamic constants rather than the F-16's per-vehicle config, and
this patch approach has hit its ceiling.
