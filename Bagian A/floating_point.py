def addFloats(x, y):
    # Ekstraksi komponen dari floating point x
    signX = extractSign(x)
    exponentX = extractExponent(x)
    mantissaX = extractMantissa(x)

    # Ekstraksi komponen dari floating point y
    signY = extractSign(y)
    exponentY = extractExponent(y)
    mantissaY = extractMantissa(y)

    # Normalisasi eksponen
    if exponentX > exponentY:
        shift = exponentX - exponentY
        mantissaY = mantissaY >> shift
        exponentY = exponentX
    else:
        shift = exponentY - exponentX
        mantissaX = mantissaX >> shift
        exponentX = exponentY

    # Penjumlahan mantissa dengan mempertimbangkan tanda
    if signX == signY:
        mantissaResult = mantissaX + mantissaY
        resultSign = signX
    else:
        if mantissaX > mantissaY:
            mantissaResult = mantissaX - mantissaY
            resultSign = signX
        else:
            mantissaResult = mantissaY - mantissaX
            resultSign = signY

    # Normalisasi hasil mantissa
    if mantissaResult & (1 << 23) == 0:
        mantissaResult = mantissaResult << 1
        exponentX = exponentX - 1

    # Gabungkan kembali hasilnya
    result = combineFloat(resultSign, exponentX, mantissaResult)
    return result

def extractSign(f):
    return (f >> 31) & 0b1

def extractExponent(f):
    return (f >> 23) & 0xFF

def extractMantissa(f):
    return f & 0x7FFFFF

def combineFloat(sign, exponent, mantissa):
    return (sign << 31) | (exponent << 23) | mantissa

print(addFloats(1.8, 1.3))