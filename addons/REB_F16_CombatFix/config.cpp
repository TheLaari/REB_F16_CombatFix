/*
    REB_F16_CombatFix
    ------------------
    A drop-in compatibility patch for:
        REBalanced - FIR F-16   (CfgPatches name: REB_FIR_F16)
        AWESome                 (aerodynamics addon)

    WHAT THIS DOES
    --------------
    This does NOT replace or re-declare the REBalanced F-16 classes. It
    "reopens" the two existing base classes (FIR_F16_Base, FIR_F16D_Base)
    that every F-16 variant in the pack inherits from, and overrides just
    three properties on each. Every skin/variant (Blk 40/50/52, ROKAF,
    Polish, WP, HO, CFT, etc.) inherits these automatically because none
    of them redefine these three fields themselves - confirmed against
    the current REBalanced source (addons/REB_FIR_F16/CfgVehicles.hpp).

    Changes made, and why (see write-up this patch is based on):

    1) airFrictionCoefs2[] forward term reduced ~15-25%
       AWESome reuses this exact coefficient as the basis of its
       Mach-dependent wave-drag term. The original REBalanced value
       (0.00001) is fine for Arma's stock air-friction model, but once
       AWESome's induced/parasite/wave-drag stack samples it, transonic
       drag balloons hard around 1200 km/h. This is the primary fix for
       "hitting a wall" under afterburner.

    2) envelope[] lift-coefficient curve raised slightly and flattened
       so it doesn't fall off quite as steeply past its peak (index 8).
       Combined with AWESome's induced-drag-scales-with-lift-squared
       behavior, the original curve's steep fall-off after Mach ~0.9
       equivalent AoA index caused the "pull -> bleed energy -> stall"
       spiral in sustained turns.

    3) stallSpeed lowered slightly to match the adjusted envelope.
       Note: stallSpeed is mostly used for AI/instruments/warnings, not
       a hard physical switch - the real behavior comes from the lift/
       drag curves above, so this alone would do nothing without (1)+(2).

    DELIBERATELY UNCHANGED
    -----------------------
    - maxSpeed, thrustCoef, refThrust/milThrust/abThrust, abThrottle,
      abFuelMultiplier: the F110-GE-129 thrust curve is already strong
      through the transonic band (~1.86 at 1200 km/h, climbing to ~1.99
      around 1600-1800 km/h) - the engine was never the bottleneck, so
      it's left alone to keep this a clean, isolated experiment.
    - Nothing about AWESome itself is touched (its global waveCd/lift/
      drag multipliers live in that addon, not here). If this patch
      doesn't fully resolve things, that's the next place to look.

    REQUIREMENTS / LOAD ORDER
    --------------------------
    Requires REB_FIR_F16 (REBalanced - FIR F-16) to be present. The
    requiredAddons[] entry below guarantees this patch's class-merge
    happens after REBalanced's own config is loaded, regardless of
    where you drag it in the Launcher's mod list.
*/

class CfgPatches
{
    class REB_F16_CombatFix
    {
        name = "REBalanced F-16 Combat Fix";
        author = "User patch";
        requiredVersion = 0.1;
        requiredAddons[] = {"REB_FIR_F16"};
        units[] = {};
        weapons[] = {};
    };
};

class CfgVehicles
{
    // Plane_Fighter_03_base_F is the true grandparent (vanilla Jets DLC
    // class) that FIR_F16_Base/FIR_F16D_Base inherit from in REBalanced's
    // own source. Forward-declaring it here (matching REBalanced's own
    // CfgVehicles.hpp pattern exactly) lets us name it as the parent
    // below.
    //
    // CRITICAL: reopening FIR_F16_Base/FIR_F16D_Base WITHOUT repeating
    // ": Plane_Fighter_03_base_F" does not just "add properties" - Arma's
    // addon merge logs confirmed it actually resets the class's parent
    // link to empty ('Plane_Fighter_03_base_F' -> ''), stripping every
    // inherited property (model, damage handling, PhysX, crew slots -
    // everything not literally re-typed inside FIR_F16_Base's own body).
    // That is what caused the ghost/no-model F-16. Repeating the parent
    // here keeps the inheritance chain intact while REBalanced's own
    // already-merged properties on this class (model, scope, etc., set
    // directly in its body) are preserved because it's the same class
    // being reopened, not replaced.
    class Plane_Fighter_03_base_F;

    // --- Single-seat F-16C base (Blk 50 and all C-model variants) ---
    class FIR_F16_Base : Plane_Fighter_03_base_F
    {
        // Original: {0.001, 0.004, 0.00001}
        airFrictionCoefs2[] = {0.00085, 0.00340, 0.0000075};

        // Original: {0,0.3,0.9,1.8,2.6,3.2,3.6,3.8,3.9,3.7,3.4,3,2.7,2.5,2.45,2.4,2.4,2.4,2.4,2.4,2.4}
        envelope[] = {
            0, 0.35, 1.00, 1.95, 2.75, 3.35, 3.75, 3.95, 4.05,
            3.95, 3.70, 3.40, 3.10, 2.85, 2.70, 2.65, 2.65, 2.65,
            2.65, 2.65, 2.65
        };

        // Original: 220
        stallSpeed = 205;
    };

    // --- Two-seat F-16D base (Blk 50 and all D-model variants) ---
    class FIR_F16D_Base : Plane_Fighter_03_base_F
    {
        airFrictionCoefs2[] = {0.00085, 0.00340, 0.0000075};

        envelope[] = {
            0, 0.35, 1.00, 1.95, 2.75, 3.35, 3.75, 3.95, 4.05,
            3.95, 3.70, 3.40, 3.10, 2.85, 2.70, 2.65, 2.65, 2.65,
            2.65, 2.65, 2.65
        };

        stallSpeed = 205;
    };
};
