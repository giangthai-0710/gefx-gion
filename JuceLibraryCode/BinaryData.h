/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Switch_pressed_png;
    const int            Switch_pressed_pngSize = 3395;

    extern const char*   Switch_png;
    const int            Switch_pngSize = 3493;

    extern const char*   LED_on_png;
    const int            LED_on_pngSize = 1306;

    extern const char*   LED_on_2_png;
    const int            LED_on_2_pngSize = 1268;

    extern const char*   LED_off_png;
    const int            LED_off_pngSize = 605;

    extern const char*   LED_changeable_png;
    const int            LED_changeable_pngSize = 14045;

    extern const char*   Knob_large_png;
    const int            Knob_large_pngSize = 132511;

    extern const char*   Knob_medium_png;
    const int            Knob_medium_pngSize = 66642;

    extern const char*   Knob_small_png;
    const int            Knob_small_pngSize = 48940;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 9;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
