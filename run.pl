#!/usr/bin/perl

open(infile,"dir_list");

while(<infile>){
    $dir = $_;
    chomp($dir);
    if (-e "./processed_data/$dir"){
    }else{
	system("mkdir ./processed_data/$dir");
    }
    system("rm -f ./wire_sim/* ./txt/*");
    system("cp ./orig_data/$dir\/* ./wire_sim");
    system("ls ./wire_sim/*.dat > filelist");
    system("./organize.pl");
    system("rm -f filelist");
    system("mv ./txt/* ./processed_data/$dir\/");
}
