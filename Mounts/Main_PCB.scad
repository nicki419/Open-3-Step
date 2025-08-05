$fn = $preview ? 0 : 100;

include <_modules.scad>;
include <_generic_board.scad>;

length = 83;
width = 32;
height = 2;

mounting_hole_length = 7;
mounting_hole_width = 8;

screw_hole_x = 16;
screw_hole_y = 15;

difference()
{
    difference()
    {
        generic_board(length, width, height, mounting_hole_length, mounting_hole_width);
        union()
        {
            translate([screw_hole_x, screw_hole_y, 0])
            wood_hole();
            
            translate([length - screw_hole_x, screw_hole_y, 0])
            wood_hole();
        }
    }
    union()
    {
        translate([0, 6.5, height])
        cube([length, width - 13, 3]);
        
        translate([6, 0, 2])
        cube([length - 12, width, 3]);
    }
}

