$fn = $preview ? 0 : 100;

include <_modules.scad>;
include <_generic_board.scad>;

length = 53;
width = 27;
height = 2;

mounting_hole_length = 6;
mounting_hole_width = 6;

screw_hole_x = 20;
screw_hole_y = 5;

difference()
{
    generic_board_m3(length, width, height, mounting_hole_length, mounting_hole_width);
    union()
    {
        translate([screw_hole_x, screw_hole_y, 0])
        wood_hole();
        
        translate([length - screw_hole_x, width - screw_hole_y, 0])
        wood_hole();
    }
}