module mounting_hole(w, l, h=3) 
{
    hole_dia = 3.0;           // slightly undersized for 3.2mm insert
    hole_depth = 2.2;         // slightly deeper than insert
    chamfer_depth = 0.5;      // how deep the chamfer is
    chamfer_top_dia = 3.4;    // wider top to guide insert

    // Create the main hole and chamfer at the top
    difference() 
    {
        translate([-(w/2),-(l/2),0])
        {
            cube([w,l,h]);
        }
        translate([0,0,3+(h-3)+.01]) 
        {
            rotate([180,0,0]) 
            {
                union() {
                    // Main cylindrical hole
                    cylinder(d = hole_dia, h = hole_depth, $fn = 64);

                    // Top chamfered cone
                    cylinder(d1 = chamfer_top_dia, d2 = hole_dia, h = chamfer_depth, $fn = 64);
                }
            }
        }
    }
}

module mounting_hole_m3(w, l, h=3) 
{
    hole_dia = 4.2;           // slightly undersized for 3.2mm insert
    hole_depth = 3;         // slightly deeper than insert
    chamfer_depth = 0.5;      // how deep the chamfer is
    chamfer_top_dia = 4.8;    // wider top to guide insert

    // Create the main hole and chamfer at the top
    difference() 
    {
        translate([-(w/2),-(l/2),0])
        {
            cube([w,l,h]);
        }
        translate([0,0,3+(h-3)+.01]) 
        {
            rotate([180,0,0]) 
            {
                union() {
                    // Main cylindrical hole
                    cylinder(d = hole_dia, h = hole_depth, $fn = 64);

                    // Top chamfered cone
                    cylinder(d1 = chamfer_top_dia, d2 = hole_dia, h = chamfer_depth, $fn = 64);
                }
            }
        }
    }
}

module wood_hole()
{
    hole_dia = 3;
    head_dia = 6;
    hole_depth = 3;
    
    translate([0,0,2.01]) 
    {
        rotate([180,0,0]) 
        union() {
            // Main cylindrical hole
            cylinder(d = hole_dia, h = hole_depth, $fn = 64);

            // Top chamfered cone
            cylinder(d1 = head_dia, d2 = hole_dia, h = 1.25, $fn = 64);
        }
    }
}
