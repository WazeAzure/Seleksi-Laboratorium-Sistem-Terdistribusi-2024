# Given data
ct1_hex = 'b6e3251da5c15ebc55f7416aa42ccb879e787425401eb5e6e2ff2572a164b64f5cebd4e9165a3e201b4dc2338928a3b6ac8243f5'  # Replace with the actual hex string of ct1
ct2_hex = 'b6e23f02a1dd4ab30ab97b5abd70d09bb0483f7d7632bcc1f2c17a509f63d9307cd7cfe736590e081d3dc46fb018a0e9a38856e3f812b2d7af6669d4edd0ab'  # Replace with the actual hex string of ct2
leak = b'Shikanoko nokonoko koshitantan! Shikanoko nokonoko koshitantan!'

# Convert the hex strings to bytes
ct1 = bytes.fromhex(ct1_hex)
ct2 = bytes.fromhex(ct2_hex)

# Derive the keystream from ct2 and the known plaintext leak
keystream = bytes([p ^ c for p, c in zip(leak, ct2)])

# Use the keystream to decrypt ct1 to get the original FLAG
decrypted_flag = bytes([c ^ k for c, k in zip(ct1, keystream[:len(ct1)])])

print(f'decrypted_flag: {decrypted_flag.decode()}')

# Sister{d0nT_r3usE_k3Y_aNd_1V_iN_sTreAm_CiPh3R_m0de5}