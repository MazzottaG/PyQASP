input = """
1 2 0 0
1 3 0 0
1 4 0 0
1 5 0 0
1 6 0 0
1 7 0 0
1 8 0 0
1 9 0 0
1 10 0 0
1 11 2 1 12 13
1 12 2 1 11 13
1 13 0 0
1 14 2 1 15 16
1 15 2 1 14 16
1 16 0 0
1 17 2 1 18 19
1 18 2 1 17 19
1 19 0 0
1 20 2 1 21 22
1 21 2 1 20 22
1 22 0 0
1 23 2 1 24 25
1 24 2 1 23 25
1 25 0 0
1 26 2 1 27 28
1 27 2 1 26 28
1 28 0 0
1 29 2 1 30 31
1 30 2 1 29 31
1 31 0 0
1 32 2 1 33 34
1 33 2 1 32 34
1 34 0 0
5 36 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 37 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 35 2 1 37 36
5 39 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 40 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 38 2 1 40 39
5 41 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 42 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 38 2 1 42 41
5 44 9 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 43 1 1 44
5 46 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 45 1 1 46
5 48 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 47 1 1 48
5 50 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 49 1 1 50
5 52 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 51 1 0 52
5 54 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 53 1 0 54
5 56 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 55 1 0 56
5 58 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 57 1 0 58
5 60 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 61 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 59 2 1 61 60
5 63 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 64 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 62 2 1 64 63
5 65 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 66 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 62 2 1 66 65
5 68 9 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 67 1 1 68
5 70 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 69 1 1 70
5 72 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 71 1 1 72
5 74 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 73 1 1 74
5 76 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 75 1 0 76
5 78 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 77 1 0 78
5 80 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 79 1 0 80
5 82 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 81 1 0 82
5 84 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 85 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 83 2 1 85 84
5 87 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 88 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 86 2 1 88 87
5 89 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 90 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 86 2 1 90 89
5 92 9 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 91 1 1 92
5 94 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 93 1 1 94
5 96 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 95 1 1 96
5 98 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 97 1 1 98
5 100 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 99 1 0 100
5 102 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 101 1 0 102
5 104 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 103 1 0 104
5 106 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 105 1 0 106
5 108 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 109 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 107 2 1 109 108
5 111 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 112 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 110 2 1 112 111
5 113 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 114 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 110 2 1 114 113
5 116 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 115 1 0 116
5 118 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 117 1 0 118
5 120 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 119 1 0 120
5 122 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 121 1 0 122
5 124 9 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 123 1 1 124
5 126 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 125 1 1 126
5 128 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 127 1 1 128
5 130 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 129 1 1 130
5 132 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 133 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 131 2 1 133 132
5 135 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 136 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 134 2 1 136 135
5 137 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 138 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 134 2 1 138 137
5 140 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 139 1 0 140
5 142 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 141 1 0 142
5 144 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 143 1 0 144
5 146 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 145 1 0 146
5 148 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 147 1 1 148
5 150 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 149 1 1 150
5 152 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 151 1 1 152
5 154 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 153 1 1 154
5 156 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 157 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 155 2 1 157 156
5 159 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 160 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 158 2 1 160 159
5 161 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 162 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 158 2 1 162 161
5 164 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 163 1 0 164
5 166 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 165 1 0 166
5 168 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 167 1 0 168
5 170 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 169 1 0 170
5 172 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 171 1 1 172
5 174 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 173 1 1 174
5 176 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 175 1 1 176
5 178 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 177 1 1 178
5 180 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 181 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 179 2 1 181 180
5 183 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 184 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 182 2 1 184 183
5 185 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 186 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 182 2 1 186 185
5 188 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 189 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 187 2 1 189 188
5 191 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 192 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 190 2 1 192 191
5 193 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 194 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 190 2 1 194 193
5 196 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 197 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 195 2 1 197 196
5 199 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 200 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 198 2 1 200 199
5 201 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 202 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 198 2 1 202 201
5 204 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 205 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 203 2 1 205 204
5 207 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 208 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 206 2 1 208 207
5 210 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 211 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 209 2 1 211 210
5 213 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 214 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 212 2 1 214 213
5 216 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 217 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 215 2 1 217 216
5 219 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 220 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 218 2 1 220 219
5 222 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 223 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 221 2 1 223 222
5 225 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 226 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 224 2 1 226 225
5 227 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 228 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 224 2 1 228 227
5 230 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 231 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 229 2 1 231 230
5 233 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 234 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 232 2 1 234 233
5 235 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 236 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 232 2 1 236 235
5 238 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 239 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 237 2 1 239 238
5 241 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 242 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 240 2 1 242 241
5 244 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 245 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 243 2 1 245 244
5 247 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 248 9 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 246 2 1 248 247
5 250 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 251 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 249 2 1 251 250
5 253 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 254 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 252 2 1 254 253
5 255 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 256 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 252 2 1 256 255
5 258 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 259 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 257 2 1 259 258
5 261 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 262 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 260 2 1 262 261
5 264 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 265 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 263 2 1 265 264
5 267 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 268 9 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 266 2 1 268 267
5 270 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 271 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 269 2 1 271 270
5 273 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 274 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 272 2 1 274 273
5 275 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 276 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 272 2 1 276 275
5 278 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 279 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 277 2 1 279 278
5 281 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 282 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 280 2 1 282 281
5 284 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 285 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 283 2 1 285 284
5 287 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 288 9 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 286 2 1 288 287
5 290 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 291 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 289 2 1 291 290
5 293 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 294 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 292 2 1 294 293
5 295 8 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 296 9 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 292 2 1 296 295
5 297 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 298 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 1 2 1 298 297
5 299 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 300 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 1 2 1 300 299
5 301 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 1 1 1 301
5 302 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 303 6 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 1 2 1 303 302
5 304 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 305 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 1 2 1 305 304
5 306 5 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 307 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 1 2 1 307 306
5 308 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 309 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 1 2 1 309 308
5 310 4 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
5 311 7 8 0 32 29 26 23 20 17 14 11 1 2 3 4 5 6 7 8
1 1 2 1 311 310
0
129 ouch20
134 ouch21
110 ouch16
141 ouch22
117 ouch17
145 ouch23
121 ouch18
99 okay14
125 ouch19
212 ouch36
3 c(1)
4 c(2)
5 c(3)
6 c(4)
7 c(5)
8 c(6)
9 c(7)
10 c(8)
240 ouch40
232 ouch39
198 ouch34
190 ouch33
206 ouch35
224 ouch38
103 okay15
91 okay12
95 okay13
175 okay31
51 okay4
218 ouch37
55 okay5
59 okay6
179 okay32
167 okay29
171 okay30
147 okay24
67 okay7
155 okay26
163 okay28
151 okay25
263 okay44
286 ouch47
71 okay8
79 okay10
257 okay43
83 okay11
2 p
75 okay9
249 okay42
243 okay41
277 okay46
43 okay2
47 okay3
269 okay45
127 okay20
131 okay21
107 okay16
139 okay22
11 d(8)
14 d(7)
17 d(6)
20 d(5)
23 d(4)
26 d(3)
29 d(2)
32 d(1)
115 okay17
143 okay23
12 n_d(8)
15 n_d(7)
18 n_d(6)
21 n_d(5)
24 n_d(4)
27 n_d(3)
30 n_d(2)
33 n_d(1)
119 okay18
123 okay19
209 okay36
237 okay40
101 ouch14
229 okay39
203 okay35
195 okay34
93 ouch12
187 okay33
97 ouch13
221 okay38
105 ouch15
53 ouch4
177 ouch31
57 ouch5
215 okay37
169 ouch29
62 ouch6
173 ouch30
182 ouch32
149 ouch24
69 ouch7
153 ouch25
158 ouch27
266 ouch44
165 ouch28
289 okay48
283 okay47
73 ouch8
260 ouch43
35 okay1
292 ouch48
81 ouch10
77 ouch9
86 ouch11
252 ouch42
38 ouch1
246 ouch41
280 ouch46
272 ouch45
45 ouch2
49 ouch3
0
B+
0
B-
1
0
1
"""
output = """
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), n_d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), d(5), n_d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), n_d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), d(6), n_d(5), n_d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), n_d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), d(5), n_d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), n_d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), d(7), n_d(6), n_d(5), n_d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), n_d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), d(5), n_d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), n_d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), d(6), n_d(5), n_d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), n_d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), d(5), n_d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), n_d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), d(8), n_d(7), n_d(6), n_d(5), n_d(4), n_d(3), n_d(2), n_d(1), okay2, okay4, ouch4, okay5, ouch5, okay7, okay9, ouch9, okay10, ouch10, okay12, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay19, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29, ouch41, ouch44, ouch47, ouch48}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), n_d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), d(5), n_d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), n_d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), d(6), n_d(5), n_d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), n_d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), d(5), n_d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), n_d(4), n_d(3), n_d(2), d(1), okay2, okay4, ouch4, okay5, ouch5, okay7, okay9, ouch9, okay10, ouch10, okay12, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay19, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29, ouch41, ouch44, ouch47, ouch48}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), n_d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), n_d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), d(5), n_d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), n_d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), n_d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), n_d(4), n_d(3), d(2), n_d(1), okay2, okay4, ouch4, okay5, ouch5, okay7, okay9, ouch9, okay10, ouch10, okay12, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay19, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29, ouch41, ouch44, ouch47, ouch48}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), d(4), d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), d(4), n_d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), d(4), n_d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), d(4), n_d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), d(4), n_d(3), n_d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), n_d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), n_d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), n_d(4), d(3), n_d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), n_d(4), d(3), n_d(2), n_d(1), okay2, okay4, ouch4, okay5, ouch5, okay7, okay9, ouch9, okay10, ouch10, okay12, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay19, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29, ouch41, ouch44, ouch47, ouch48}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), n_d(4), n_d(3), d(2), d(1), okay2, okay4, ouch4, okay5, ouch5, okay7, okay9, ouch9, okay10, ouch10, okay12, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay19, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29, ouch41, ouch44, ouch47, ouch48}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), n_d(5), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), n_d(5), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), n_d(5), d(4), d(3), n_d(2), d(1), okay2, okay4, ouch4, okay5, ouch5, okay7, okay9, ouch9, okay10, ouch10, okay12, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay19, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29, ouch41, ouch44, ouch47, ouch48}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), d(7), n_d(6), n_d(5), n_d(4), n_d(3), n_d(2), n_d(1), ouch1, okay2, ouch2, okay4, ouch4, okay5, ouch5, ouch6, okay7, ouch7, okay9, ouch9, okay10, ouch10, ouch11, okay12, ouch12, okay14, ouch14, okay15, ouch15, ouch16, okay17, ouch17, okay18, ouch18, okay19, ouch19, ouch21, okay22, ouch22, okay23, ouch23, ouch27, okay28, ouch28, okay29, ouch29, ouch35, ouch36, ouch37, ouch38, ouch39, okay41, ouch41, ouch42, okay44, ouch44, ouch45, okay47, ouch47}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), d(6), n_d(5), n_d(4), n_d(3), n_d(2), d(1), ouch1, okay2, ouch2, okay4, ouch4, okay5, ouch5, ouch6, okay7, ouch7, okay9, ouch9, okay10, ouch10, ouch11, okay12, ouch12, okay14, ouch14, okay15, ouch15, ouch16, okay17, ouch17, okay18, ouch18, okay19, ouch19, ouch21, okay22, ouch22, okay23, ouch23, ouch27, okay28, ouch28, okay29, ouch29, ouch35, ouch36, ouch37, ouch38, ouch39, okay41, ouch41, ouch42, okay44, ouch44, ouch45, okay47, ouch47}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), d(5), n_d(4), n_d(3), d(2), n_d(1), ouch1, okay2, ouch2, okay4, ouch4, okay5, ouch5, ouch6, okay7, ouch7, okay9, ouch9, okay10, ouch10, ouch11, okay12, ouch12, okay14, ouch14, okay15, ouch15, ouch16, okay17, ouch17, okay18, ouch18, okay19, ouch19, ouch21, okay22, ouch22, okay23, ouch23, ouch27, okay28, ouch28, okay29, ouch29, ouch35, ouch36, ouch37, ouch38, ouch39, okay41, ouch41, ouch42, okay44, ouch44, ouch45, okay47, ouch47}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), n_d(5), d(4), d(3), n_d(2), n_d(1), ouch1, okay2, ouch2, okay4, ouch4, okay5, ouch5, ouch6, okay7, ouch7, okay9, ouch9, okay10, ouch10, ouch11, okay12, ouch12, okay14, ouch14, okay15, ouch15, ouch16, okay17, ouch17, okay18, ouch18, okay19, ouch19, ouch21, okay22, ouch22, okay23, ouch23, ouch27, okay28, ouch28, okay29, ouch29, ouch35, ouch36, ouch37, ouch38, ouch39, okay41, ouch41, ouch42, okay44, ouch44, ouch45, okay47, ouch47}
{p, c(1), c(2), c(3), c(4), c(5), c(6), c(7), c(8), n_d(8), n_d(7), n_d(6), n_d(5), d(4), n_d(3), d(2), d(1), ouch1, okay2, ouch2, okay4, ouch4, okay5, ouch5, ouch6, okay7, ouch7, okay9, ouch9, okay10, ouch10, ouch11, okay12, ouch12, okay14, ouch14, okay15, ouch15, ouch16, okay17, ouch17, okay18, ouch18, okay19, ouch19, ouch21, okay22, ouch22, okay23, ouch23, ouch27, okay28, ouch28, okay29, ouch29, ouch35, ouch36, ouch37, ouch38, ouch39, okay41, ouch41, ouch42, okay44, ouch44, ouch45, okay47, ouch47}
"""
