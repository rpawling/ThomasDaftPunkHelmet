/***********************************************************************
 * Data array for Thomas Daft Punk helmet to animate EKG line
 * 
 * Generated by LED Matrix Studio
 * Settings:
 * - Run export
 * - c code (2D array)
 * - Rows
 * - Top to Bottom
 * - Right to Left
 * - LSB (Right)
 * - Hex
 * - 8bit
 * - verify that output matches hex readout from editor!
 * 
 * Authors: Ryan Pawling and Chelsea Pawling
 *
 * Adapted for 6x32 display
 ***********************************************************************/
#define EKG_FRAMES 32
#define EKG_ROWS 6
#define EKG_DELAY 25

byte ekg_array[][4] = {
                     {0x00, 0x40, 0x01, 0x00},
                     {0x00, 0xE0, 0x83, 0x80},
                     {0xF1, 0xB1, 0xC6, 0xC7},
                     {0x1B, 0x1B, 0x6C, 0x6C},
                     {0x0E, 0x0E, 0x38, 0x38},
                     {0x04, 0x04, 0x10, 0x10},
                     };
// 
// =================================================================
// 

