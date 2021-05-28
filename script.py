import matplotlib.pyplot as plt
plt.xlabel("сигнал/шум, db")
plt.ylabel("вероятность ошибки на кодовое слово")

plt.yscale('log')

# a = [1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,3.2,3.4]
# b1 = [0.42955,0.33095,0.23815,0.1727,0.11495,0.07315,0.04485,0.02385,0.01275,0.00815,0.00295,0.00145]
# b2 = [0.20825,0.1384,0.0915,0.05085,0.0292,0.01405,0.006,0.00275,0.001,0.00045,0.0002,5e-05]

# a = [1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,3.2,3.4,3.6,3.8,4,4.2]
# b1 = [0.4314,0.3406,0.2634,0.1993,0.1448,0.0987,0.067,0.0444,0.0282,0.0192,0.0132,0.0075,0.0041,0.0023,0.0015,0.0009]
# b2 = [0.3921,0.3056,0.2146,0.1421,0.0924,0.0569,0.0299,0.0202,0.0097,0.0059,0.0032,0.001,0.0006,0.0003,0.0001,0.0001]
# b3 = [0.3368,0.2556,0.1797,0.1267,0.0835,0.057,0.0371,0.0217,0.015,0.0089,0.0044,0.0035,0.0019,0.0009,0.0008,0.0004]
# b4 = [0.3733,0.2667,0.1818,0.112,0.0646,0.0332,0.0153,0.0069,0.0032,0.0016,0.0006,0.0002,0,0,0,0]

# a = [1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,3.2,3.4]
# b1 = [0.42869,0.34111,0.26193,0.2002,0.14267,0.10243,0.06794,0.04652,0.031,0.01857,0.01252,0.00759]
# b2 = [0.40158,0.3031,0.21093,0.14522,0.09175,0.05654,0.03348,0.01805,0.00976,0.00516,0.00284,0.00122]
# b3 = [0.28429,0.19991,0.13061,0.08047,0.04739,0.02651,0.01429,0.00739,0.00379,0.0019,0.00081,0.0004]
# b4 = [0.33455,0.25489,0.18425,0.12999,0.08804,0.05878,0.03672,0.02319,0.01427,0.00868,0.00549,0.00359]
# b5 = [0.37044,0.2694,0.18137,0.11313,0.06699,0.03546,0.01681,0.00759,0.00297,0.00125,0.0005,0.00017]
# b6 = [0.20651,0.13112,0.07876,0.04336,0.0223,0.01041,0.00464,0.00187,0.00083,0.00037,0.00018,7e-05]

# a = [1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,3.2]
# b51 = [0.31278,0.22087,0.14677,0.09131,0.05203,0.0279,0.0144,0.00652,0.00324,0.00166,0.00063]
# b61 = [0.20707,0.13295,0.07984,0.04389,0.02341,0.01071,0.0049,0.00216,0.00092,0.00058,0.0002]
# b52 = [0.37044,0.2694,0.18137,0.11313,0.06699,0.03546,0.01681,0.00759,0.00297,0.00125,0.0005]
# b62 = [0.20651,0.13112,0.07876,0.04336,0.0223,0.01041,0.00464,0.00187,0.00083,0.00037,0.00018]

# a = [1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,3.2,3.4,3.6,3.8,4,4.2]
# b1 = [0.4276,0.3401,0.2662,0.1986,0.1396,0.1013,0.0713,0.0484,0.0327,0.0187,0.0122,0.0082,0.0044,0.0027,0.0016,0.0011]
# b2 = [0.3996,0.2935,0.2166,0.1491,0.0975,0.0574,0.0345,0.0196,0.0093,0.0058,0.0024,0.0019,0.0009,0.0003,0.0002,0]
# b3 = [0.3392,0.2563,0.188,0.1267,0.0898,0.0564,0.0389,0.0249,0.0158,0.0095,0.0058,0.003,0.0017,0.0011,0.0004,0.0004]
# b4 = [0.3835,0.2703,0.1917,0.1231,0.0712,0.0381,0.0193,0.0078,0.0047,0.0009,0.0004,0,0.0001,0,0,0]

# a = [1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,3.2]
# b1 = [0.4312,0.3414,0.2615,0.2021,0.1398,0.1021,0.0746,0.0487,0.0299,0.0226,0.0135,]
# b2 = [0.3953,0.2861,0.2054,0.1298,0.0809,0.0442,0.0257,0.0133,0.0057,0.003,0.0009]

a = [1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,3.2,3.4]
# b1 = [0.42615,0.34058,0.26495,0.20025,0.14293,0.09989,0.07038,0.0469,0.03018,0.01882,0.01255,0.00756]
# b2 = [0.38566,0.28241,0.19789,0.12787,0.0782,0.04538,0.02266,0.01224,0.00586,0.00271,0.00117,0.00048]
# b3 = [0.26808,0.17999,0.11373,0.06671,0.03628,0.01839,0.00825,0.00389,0.00161,0.00062,0.00028,0.00012]
b4 = [0.336,0.25511,0.18673,0.12945,0.08833,0.05872,0.03692,0.02273,0.01412,0.00907,0.00547,0.00317]
b5 = [0.31278,0.22087,0.14677,0.09131,0.05203,0.0279,0.0144,0.00652,0.00324,0.00166,0.00063,0.00033]
b6 = [0.20707,0.13295,0.07984,0.04389,0.02341,0.01071,0.0049,0.00216,0.00092,0.00058,0.0002,7e-05]



# line1, = plt.plot(a, b1)
# line1.set_label('(256, 128), SC')
#
# line2, = plt.plot(a, b2)
# line2.set_label('(256, 128+10), SCFlip T = 10')
#
# line3, = plt.plot(a, b3)
# line3.set_label('(256, 128+10), 16x16 kernel, SC')
#
line4, = plt.plot(a, b4)
line4.set_label('(256, 128+10), 16x16 kernel, SC')

line5, = plt.plot(a, b5)
line5.set_label('(256, 128+10), 16x16 kernel, SCFlip T = 10')

line6, = plt.plot(a, b6)
line6.set_label('(256, 128+10), 16x16 kernel, SCFlip T = 50')

# line7, = plt.plot(a, b52)
# line7.set_label('(256, 128+10), 16x16 kernel, FastSCFlip T = 10')

# line8, = plt.plot(a, b62)
# line8.set_label('(256, 128+10), 16x16 kernel, FastSCFlip T = 50')


plt.legend()
plt.show()
