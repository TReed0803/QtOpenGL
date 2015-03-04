#!/usr/bin/env perl

# GenHeader.pl
# Generates headers for all C++ headers which define a variable GENHEADER. Will
# create a header which is the name of what GENHEADER is defined as, and will
# simply include the file which defined GENHEADER.

use warnings;
use strict;
use Cwd;
use File::Find;
use File::Path qw( make_path );

my $cwd = getcwd;
my $outDir = $cwd . "/include/Karma/";

sub make_link
{
  my ($basename, $name) = @_;
  my $filepath = $outDir . $name;
  unless (-e $filepath)
  {
    print "Creating $filepath...\n";
    open my $file, ">", $filepath or die $!;
    print $file "#include \"$basename\"";
    close $file;
  }
}

sub parse_header
{
  my ( $basename, $filename ) = @_;

  open my $file, "<", $filename or die $!;
  while (<$file>)
  {
    if (/^[\s]*#[\s]*define[\s]+[a-zA-Z_0-9]*_H[\s]+(.*)/)
    {
      make_link( $basename, $1);
    }
  }
  close $file;
}

sub parse_all_headers
{
  my $filename = $File::Find::name;
  if ($filename =~ /\.h$/)
  {
    parse_header($_, $filename);
  }
}

unless ( -d $outDir )
{
  print "Creating `$outDir`...";
  make_path $outDir or die "Failed to create path: $outDir";
}
find(\&parse_all_headers, $cwd);
