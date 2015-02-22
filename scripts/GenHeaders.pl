#!/usr/bin/env perl

# GenHeader.pl
# Generates headers for all C++ headers which define a variable GENHEADER. Will
# create a header which is the name of what GENHEADER is defined as, and will
# simply include the file which defined GENHEADER.

use warnings;
use strict;
use Cwd;
use File::Find;

my $cwd = getcwd;

sub make_link
{
  my ($directory, $basename, $name) = @_;
  my $filepath = $directory . "/" . $name;
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
  my ( $directory, $basename, $filename ) = @_;

  open my $file, "<", $filename or die $!;
  while (<$file>)
  {
    if (/^[\s]*#[\s]*define[\s]+[a-zA-Z_0-9]*_H[\s]+(.*)/)
    {
      make_link($directory, $basename, $1);
    }
  }
  close $file;
}

sub parse_all_headers
{
  my $filename = $File::Find::name;
  if ($filename =~ /\.h$/)
  {
    parse_header($File::Find::dir, $_, $filename);
  }
}

find(\&parse_all_headers, $cwd);
