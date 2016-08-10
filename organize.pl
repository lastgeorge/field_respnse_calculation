#!/usr/bin/perl

open(infile,"filelist");
while(<infile>){
    $filename = $_;
    chomp($filename);
    
    open(infile1,"$filename");
    $newfile = $filename;
    $newfile =~ s/wire_sim/txt/;
    #print "$newfile\n";
    open(outfile,">$newfile");
    my $flag = 0;
    my $num = 1;
    while(<infile1>){
	my $current_line = $_;
	chomp($current_line);
	my @temp = split(/\s+/,$_);
	
	if ($current_line =~m/\)/ && $current_line =~ m/\+/){
	    $flag = 0;
	    $num++;
	}

	if ($flag==1){
	    print outfile "$num $temp[2] $temp[3]\n";
	}

	if ($current_line =~ m/\(/ && $current_line =~ m/\+/){
	    $flag = 1;
	}

	
	

    }
    close(outfile);
    close(infile1);
    
}
close(infile);
