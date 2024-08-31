use strict;
use warnings;
use integer;

# list of routines
sub myAdd {
    
    my ($a, $b) = @_;

    START_ADD:
    if($b == 0){
        goto END_ADD;
    }

    my $carry = $a & $b;
    $a = $a ^ $b;
    $b = $carry << 1;

    goto START_ADD;

    END_ADD:
    return $a; 
}

sub mySubtract {
    my ($a, $b) = @_;

    if ($b == 0) {
        return $a;
    }

    return myAdd($a, myAdd(~$b, 1));
}

sub myMultiply {
    my ($a, $b) = @_;
    my $result = 0;

    my $negative = 0;
    if ($a & (1 << 63)) {
        $a = myAdd(~$a, 1);
        $negative = !$negative;
    }
    if ($b & (1 << 63)) {
        $b = myAdd(~$b, 1);
        $negative = !$negative;
    }

    START_MULTIPLY:
    if($b <= 0){
        goto END_MULTIPLY;
    }

    if ($b & 1) {
        $result = myAdd($result, $a);
    }
    $a = $a << 1;
    $b = $b >> 1;

    goto START_MULTIPLY;

    END_MULTIPLY:
    if ($negative) {
        $result = myAdd(~$result, 1);
    }
    return $result;
}

sub myDivision {
    my ($a, $b) = @_;

    # Handle division by zero
    if ($b == 0) {
        die "Division by zero error";
    }

    my $negative = 0;
    if ($a & (1 << 63)) {
        $a = myAdd(~$a, 1);
        $negative = !$negative;
    }
    if ($b & (1 << 63)) {
        $b = myAdd(~$b, 1);
        $negative = !$negative;
    }

    my $quotient = 0;
    my $remainder = 0;
    my $i = 63;

    START_DIVISION:
    if ($i < 0) {
        goto END_DIVISION;
    }

    $remainder = $remainder << 1;
    $remainder |= ($a >> $i) & 1;
    if ($remainder >= $b) {
        $remainder = mySubtract($remainder, $b);
        $quotient |= 1 << $i;
    }
    $i = $i - 1;
    goto START_DIVISION;

    END_DIVISION:
    if ($negative) {
        $quotient = myAdd(~$quotient, 1);
    }

    return $quotient;
}

sub myPower {
    my ($a, $b) = @_;
    my $result = 1;

    START_POWER:
    if($b == 0){
        goto END_POWER;
    }

    $result = myMultiply($result, $a);
    $b = mySubtract($b, 1);

    goto START_POWER;

    END_POWER:
    return $result;
}

sub myInvSqrt {
    my ($a, $b) = @_;

    return $a + $b;
}


# main func

print "Enter the equation: \n> ";
my $input = <STDIN>;

# Initialize variables
my $result;
my $current_number = '';
my $operator;
my $index = 0;
my $len = length($input);

# Parse and evaluate the input string character by character
START_PARSE:
if ($index >= $len) {
    goto END_PARSE;
}

my $char = substr($input, $index, 1);

if ($char =~ /\d/) {
    $current_number .= $char;
    $index++;
    goto START_PARSE;
} elsif ($char =~ /[\+\-\*\^\@\/]/) {
    if (!defined $result) {
        $result = $current_number;
    } else {
        if ($operator eq '+') {
            $result = myAdd($result, $current_number);
        } elsif ($operator eq '-') {
            $result = mySubtract($result, $current_number);
        } elsif ($operator eq '*') {
            $result = myMultiply($result, $current_number);
        } elsif ($operator eq '/') {
            $result = myDivision($result, $current_number);
        } elsif ($operator eq '^') {
            $result = myPower($result, $current_number);
        } elsif ($operator eq '@') {
            print "detected\n";
            $result = myInvSqrt($result, $current_number);
        }
    }
    $operator = $char;
    $current_number = '';
    $index++;
    goto START_PARSE;
} elsif ($char eq ' ') {
    $index++;
    goto START_PARSE;
}

END_PARSE:
if (defined $operator) {
    if ($operator eq '+') {
        $result = myAdd($result, $current_number);
    } elsif ($operator eq '-') {
        $result = mySubtract($result, $current_number);
    } elsif ($operator eq '*') {
        $result = myMultiply($result, $current_number);
    } elsif ($operator eq '/') {
        $result = myDivision($result, $current_number);
    } elsif ($operator eq '^') {
        $result = myPower($result, $current_number);
    } elsif ($operator eq '@') {
        print "detected2\n";
        $result = myInvSqrt($result, $current_number);
    }
} else {
    $result = $current_number;
}

print "Result: $result\n";
