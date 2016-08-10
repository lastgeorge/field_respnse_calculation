#!/usr/bin/perl

my $name = "1.5_Y";

open(infile,"./orig/$name\.dat");
my $flag = 0;
my $prev_flag = 0;
while(<infile>){
    $current_line = $_;
    chomp($current_line);

    my @temp = split(/\s+/,$current_line);

    if ($temp[1] eq "Group"){
	$prev_flag = $flag;
	$flag = $temp[2];
	print "$prev_flag $flag\n";
    }
    
    if ($flag >0 ){
	
	open(outfile,">>$name$flag\.dat");
	
	print outfile "$current_line\n";
    }

}
