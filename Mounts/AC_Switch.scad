$fn = $preview ? 0 : 100;

include <_modules.scad>;
include <_generic_board.scad>;

length = 34;
width = 17;
height = 2;

mounting_hole_length = 4;
mounting_hole_width = 4;
mounting_hole_height = 4;

screw_hole_x = 10;
screw_hole_y = 8.5;

difference()
{
    difference()
    {
        generic_board(length, width, height, mounting_hole_length, mounting_hole_width, mounting_hole_height);
        union()
        {
            translate([screw_hole_x, screw_hole_y, 0])
            wood_hole();
            
            translate([length - screw_hole_x, screw_hole_y, 0])
            wood_hole();
        }
    }
    translate([0, 0, height]) cube([mounting_hole_length, width, mounting_hole_height]);
}

// Hook
translate([0, width/2 - mounting_hole_width/2, height]) cube([mounting_hole_width, mounting_hole_length, mounting_hole_height]);
difference()
{
    translate([-1, 0, 0]) cube([1, width, height + mounting_hole_height + 2]);
    translate([-0.5, 0, height + mounting_hole_height]) cube([0.5, width, 1.1]);
}
translate([0, width/2 - 0.25, height + mounting_hole_height + 1.1]) cube([mounting_hole_length, 0.5, 0.9]);