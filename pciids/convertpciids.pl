#!/usr/bin/perl
#
#  Copyright (c) 2021  joevt   All rights reserved.
#
#  convertpciids.pl
#
#  License: BSD 2 clause License
#

use warnings;
use strict;

my %vendors = ();
my %devices = ();
my %subsystems = ();
my %classes = ();
my %subclasses = ();
my %progifs = ();

my $vendor = "";
my $device = "";
my $subsystem = "";
my $class = "";
my $subclass = "";
my $progif = "";

sub printitems {
	my $name = $_[0];
	my $itemsref = $_[1];
	my $k;
	if (%$itemsref) {
		print "static PciIdItem " . $name . "[] = {\n";
		foreach $k (sort keys %$itemsref) {
			printf("\t{0x%s, L\"%s\"%s},\n", $k, $itemsref->{$k}[0] =~ s/([?"\\])/\\$1/gr, $itemsref->{$k}[1] ? ", Child($itemsref->{$k}[1])" : "");
		}
		print "};\n\n";
		%$itemsref = ();
	}
}

sub printall {
	my $what = $_[0];
	if ($what >= 1) { printitems("D_" . $vendor . "_" . $device , \%subsystems) }
	if ($what >= 2) { printitems("V_" . $vendor                 , \%devices   ) }
	if ($what >= 3) { printitems("Vendors"                      , \%vendors   ) }
	if ($what >= 4) { printitems("S_" . $class . "_" . $subclass, \%progifs   ) }
	if ($what >= 5) { printitems("C_" . $class                  , \%subclasses) }
	if ($what >= 6) { printitems("Classes"                      , \%classes   ) }
}

print '#include "pciidsFile.h"

#define Child(x) sizeof(x) / sizeof (PciIdItem), x
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"

';

while (<>) {
       if (0==1) {}
	elsif (/^\t\t(\w{4}) (\w{4})  (.*)/) { printall(0); $subsystem = $1 . $2; $subsystems{$subsystem} = [$3]; $devices   {$device  }[1] = "D_" . $vendor . "_" . $device  }
	elsif (/^\t(\w{4})  (.*)/          ) { printall(1); $device    = $1     ; $devices   {$device   } = [$2]; $vendors   {$vendor  }[1] = "V_" . $vendor                  }
	elsif (/^(\w{4})  (.*)/            ) { printall(2); $vendor    = $1     ; $vendors   {$vendor   } = [$2];                                                             }
	elsif (/^\t\t(\w{2})  (.*)/        ) { printall(3); $progif    = $1     ; $progifs   {$progif   } = [$2]; $subclasses{$subclass}[1] = "S_" . $class . "_" . $subclass }
	elsif (/^\t(\w{2})  (.*)/          ) { printall(4); $subclass  = $1     ; $subclasses{$subclass } = [$2]; $classes   {$class   }[1] = "C_" . $class                   }
	elsif (/^C (\w{2})  (.*)/          ) { printall(5); $class     = $1     ; $classes   {$class    } = [$2];                                                             }
}
printall(6);

print 'PciIdItem PciIdsInternal[] = {
	{0, L"Vendors", Child(Vendors)},
	{1, L"Classes", Child(Classes)},
};

VOID
UseInternalPciIds (VOID)
{
	PciIds = PciIdsInternal;
}

#pragma clang diagnostic pop
#undef Child
';
