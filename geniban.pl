#!/usr/bin/env perl
use strict;
use warnings;
use threads qw'stringify';

$" = '';

# total number of iban.exe invocations
my $tries = 9999; 

# total number of threads
my $threads = 3;

for (1 .. $threads) {
	thread_create($tries / $threads);
}

# for (1 .. $tries) {
	# my @a = gen_iban();
	# thread_create(@a);
	# # system("iban @a");
# }

$_->join() for (threads->list());

sub thread_create {#{{{

	threads->create(sub {
			my $cnt = shift;
			# sleep 5;
			for (1 .. $cnt) {
				my @a = gen_iban();
				system("./iban.exe @a");
				# `iban @a`;
			}
			threads->self()->detach()
			if (threads->self()->is_joinable());
		}, shift);
}#}}}
sub gen_iban {#{{{
	# my $iban = 'DE871234567812345678';
	my $iban = getValidIban();
	my @a = split //, $iban;
	# @a = iban_insert_spaces(@a);
	# @a = iban_add_noise(@a);
	# @a = iban_shrink(@a);
	# print "'", @a, "'\n";
	return @a;
}#}}}
sub iban_insert_spaces {#{{{
	my @a = @_;
	# my $action_qty = int(rand (127 - scalar @a));
	my $action_qty = int(126 - scalar @a);
	# my $action_qty = 1;
	for (1 .. $action_qty) {
		my $len = scalar @a - 1;
		my $action_pos = int(rand $len);
		@a = (
			@a[0			.. $action_pos	],
			'-',
			@a[$action_pos + 1	.. $len		]
		);
		# substr
	}
	return @a;
}#}}}
sub iban_add_noise {#{{{
	my @a = @_;
	# my $noise = rand 999;
	my $noise_qty = 1;
	my @noise_pat = ('a' .. 'z', 'A' .. 'Z', 0 .. 9);
	my $noise_len = scalar @noise_pat;

	for (1 .. $noise_qty) {
		my $len = scalar @a - 1;
		my $insert_position = rand $len;
		@a = (
			@a[0 .. $insert_position],
			$noise_pat[int(rand $noise_len)], 
			@a[$insert_position + 1 .. $len]
		);
	}
	return @a;
}#}}}
sub iban_shrink {#{{{
	my @a = @_;

	my $action_qty = int(rand scalar @a) - 1;

	for (1 .. $action_qty) {
		my $len = scalar @a - 1;
		my $action_pos = int rand $len;
		@a = (
			@a[0			.. $action_pos],
			@a[$action_pos + 2	.. $len]
		);
	}

	return @a;
}#}}}
sub getValidIban {#{{{

	# list taken from:
	# http://www.tbg5-finance.org/?ibandocs.shtml

	my @valid_ibans = (#{{{
		"AD1200012030200359100100",
		"AE070331234567890123456",
		"AL47212110090000000235698741",
		"AT611904300234573201",
		"AZ21NABZ00000000137010001944",
		"BA391290079401028494",
		"BE68539007547034",
		"BG80BNBG96611020345678",
		"BH67BMAG00001299123456",
		"CH9300762011623852957",
		"CR0515202001026284066",
		"CY17002001280000001200527600",
		"CZ6508000000192000145399",
		"DE89370400440532013000",
		"DK5000400440116243",
		"DO28BAGR00000001212453611324",
		"EE382200221020145685",
		"ES9121000418450200051332",
		"FI2112345600000785",
		"FO6264600001631634",
		"FR1420041010050500013M02606",
		"GB29NWBK60161331926819",
		"GE29NB0000000101904917",
		"GI75NWBK000000007099453",
		"GL8964710001000206",
		"GR1601101250000000012300695",
		"GT82TRAJ01020000001210029690",
		"HR1210010051863000160",
		"HU42117730161111101800000000",
		"IE29AIBK93115212345678",
		"IL620108000000099999999",
		"IS140159260076545510730339",
		"IT60X0542811101000000123456",
		"KW81CBKU0000000000001234560101",
		"KZ86125KZT5004100100",
		"LB62099900000001001901229114",
		"LI21088100002324013AA",
		"LT121000011101001000",
		"LU280019400644750000",
		"LV80BANK0000435195001",
		"MC5811222000010123456789030",
		"MD24AG000225100013104168",
		"ME25505000012345678951",
		"MK07250120000058984",
		"MR1300020001010000123456753",
		"MT84MALT011000012345MTLCAST001S",
		"MU17BOMM0101101030300200000MUR",
		"NL91ABNA0417164300",
		"NO9386011117947",
		"PK36SCBL0000001123456702",
		"PL61109010140000071219812874",
		"PS92PALS000000000400123456702",
		"PT50000201231234567890154",
		"RO49AAAA1B31007593840000",
		"RS35260005601001611379",
		"SA0380000000608010167519",
		"SE4550000000058398257466",
		"SI56263300012039086",
		"SK3112000000198742637541",
		"SM86U0322509800000000270100",
		"TN5910006035183598478831",
		"TR330006100519786457841326",
		"VG96VPVG0000012345678901"
	);#}}}
	return $valid_ibans[int rand scalar @valid_ibans];
}#}}}
