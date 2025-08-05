$fn = $preview ? 0 : 100;

// M3 Washers
for(i = [0 : 1])
{
    for(j = [0 : 1])
    {
        translate([i*5, j*5, 0]) washer(3);
    }
}

// M2 Washers
translate([10, 0, 0])
for(i = [0 : 5])
{
    for(j = [0 : 1])
    {
        translate([i*3.5, j*3.5, 0]) washer(2);
    }
}

module washer(size = 2)
{
    difference()
    {
        cylinder(r=size-size/4, h=1.5);
        cylinder(r=size/2, h=1.5);
    }
}