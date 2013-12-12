#ifndef SYNAPTICS_TEST_RAWDATA_H
#define SYNAPTICS_TEST_RAWDATA_H

#define TX_NUM_TRULY  (13)
#define RX_NUM_TRULY  (25)
#define DiagonalUpperLimit_TRULY  (1100)
#define DiagonalLowerLimit_TRULY  (900)

const int16_t raw_cap_data_truly_3035[TX_NUM_TRULY][RX_NUM_TRULY*2] =
{
	{1444,2407,1374,2291,1428,2381,1368,2281,1364,2273,1379,2298,1332,2221,1342,2236,1357,2261,1326,2210,1321,2201,1363,2271,1111,1852,1107,1846,1094,1823,1078,1797,1075,1791,1076,1792,1059,1765,1060,1766,1071,1785,1070,1783,1073,1788,1037,1729,1024,1707,},
	{1370,2283,1338,2230,1339,2232,1324,2207,1304,2173,1322,2204,1306,2177,1300,2167,1306,2176,1273,2122,1277,2128,1263,2106,1194,1990,1184,1973,1203,2005,1159,1932,1165,1941,1158,1930,1150,1916,1148,1914,1157,1929,1151,1919,1156,1927,1127,1878,1117,1862,},
	{1366,2277,1329,2214,1346,2243,1318,2196,1303,2172,1309,2181,1294,2156,1282,2136,1292,2153,1262,2103,1263,2105,1249,2082,1201,2002,1195,1992,1192,1986,1171,1952,1175,1959,1162,1937,1148,1914,1155,1925,1169,1948,1168,1946,1166,1943,1134,1890,1128,1879,},
	{1366,2276,1331,2218,1339,2232,1333,2222,1299,2165,1296,2161,1285,2141,1268,2113,1283,2138,1261,2102,1257,2094,1247,2078,1208,2013,1202,2004,1221,2036,1178,1963,1176,1960,1163,1938,1167,1945,1192,1986,1152,1921,1141,1901,1147,1911,1133,1889,1124,1873,},
	{1367,2278,1331,2218,1339,2231,1325,2209,1291,2152,1292,2154,1273,2122,1268,2114,1281,2135,1251,2085,1253,2089,1244,2074,1210,2017,1208,2013,1236,2059,1191,1985,1184,1973,1175,1959,1172,1954,1188,1980,1160,1933,1146,1910,1156,1927,1135,1892,1128,1881,},
	{1355,2258,1334,2223,1338,2230,1309,2182,1280,2134,1285,2142,1267,2112,1264,2108,1266,2109,1240,2066,1248,2081,1232,2052,1211,2017,1212,2020,1244,2073,1197,1995,1193,1988,1184,1973,1190,1984,1215,2025,1163,1938,1151,1918,1156,1926,1136,1892,1135,1891,},
	{1330,2217,1317,2194,1311,2185,1303,2171,1287,2145,1287,2146,1263,2106,1259,2099,1263,2105,1238,2064,1243,2071,1225,2040,1209,2015,1232,2053,1214,2023,1214,2023,1219,2030,1228,2046,1170,1950,1179,1966,1172,1954,1159,1932,1159,1932,1138,1896,1148,1913,},
	{1318,2197,1316,2194,1306,2177,1300,2166,1286,2143,1294,2157,1261,2102,1256,2094,1266,2110,1227,2045,1228,2046,1219,2032,1211,2019,1250,2083,1217,2028,1214,2023,1206,2010,1197,1995,1176,1960,1186,1976,1180,1966,1165,1941,1169,1948,1156,1927,1157,1928,},
	{1307,2178,1297,2162,1299,2164,1292,2153,1279,2131,1281,2135,1260,2100,1252,2086,1251,2084,1218,2030,1221,2033,1212,2019,1213,2022,1220,2033,1218,2030,1198,1996,1207,2010,1198,1997,1183,1972,1190,1984,1187,1978,1170,1950,1179,1966,1177,1962,1165,1942,},
	{1302,2170,1299,2165,1304,2173,1290,2150,1282,2137,1277,2128,1258,2097,1255,2092,1257,2095,1223,2038,1225,2042,1213,2022,1228,2046,1268,2113,1229,2049,1227,2045,1223,2038,1209,2015,1196,1994,1199,1999,1197,1995,1182,1969,1193,1989,1192,1987,1173,1954,},
	{1296,2160,1287,2145,1298,2163,1291,2152,1275,2125,1273,2121,1259,2099,1253,2088,1249,2081,1224,2040,1225,2042,1213,2020,1255,2091,1255,2091,1243,2071,1229,2048,1236,2059,1221,2036,1211,2019,1213,2022,1205,2007,1199,1998,1210,2016,1200,2000,1177,1961,},
	{1293,2156,1282,2136,1296,2161,1299,2165,1267,2112,1270,2116,1250,2083,1246,2077,1240,2067,1218,2029,1217,2028,1206,2009,1298,2163,1334,2224,1261,2101,1247,2079,1259,2099,1244,2074,1239,2066,1238,2063,1224,2040,1220,2033,1231,2052,1222,2036,1214,2023,},
	{1254,2090,1237,2061,1241,2068,1224,2040,1207,2012,1203,2006,1185,1975,1179,1964,1170,1950,1146,1910,1143,1905,1129,1882,1611,2684,1434,2391,1378,2297,1336,2227,1334,2223,1315,2192,1304,2174,1297,2162,1286,2143,1277,2128,1288,2146,1286,2144,1326,2210,},
};


#define TX_NUM_WINTEK  (13)
#define RX_NUM_WINTEK  (24)
#define DiagonalUpperLimit_WINTEK  (1100)
#define DiagonalLowerLimit_WINTEK  (900)

const int16_t raw_cap_data_wintek_9093[TX_NUM_WINTEK][RX_NUM_WINTEK*2] =
{
	{1555,2765,1443,2565,1459,2594,1407,2502,1404,2496,1398,2485,1383,2459,1379,2451,1373,2441,1367,2431,1370,2436,1434,2550,1191,2117,1187,2110,1184,2104,1175,2089,1167,2075,1157,2057,1150,2044,1168,2076,1161,2064,1156,2055,1180,2097,1165,2070,}, 
	{1432,2547,1382,2457,1365,2426,1353,2405,1338,2380,1328,2361,1320,2346,1307,2324,1300,2311,1292,2296,1282,2280,1286,2286,1226,2179,1218,2165,1214,2158,1237,2199,1201,2135,1204,2140,1185,2107,1200,2134,1200,2133,1193,2120,1220,2169,1242,2207,}, 
	{1402,2493,1371,2437,1361,2419,1347,2395,1336,2375,1326,2357,1316,2340,1303,2317,1296,2303,1284,2283,1273,2263,1271,2259,1232,2190,1224,2177,1216,2163,1212,2155,1202,2137,1196,2126,1188,2112,1208,2147,1206,2145,1200,2134,1226,2180,1222,2173,}, 
	{1385,2463,1366,2429,1354,2407,1344,2389,1336,2375,1321,2349,1313,2334,1302,2314,1295,2303,1287,2288,1276,2268,1269,2256,1237,2200,1231,2189,1225,2177,1236,2198,1217,2163,1225,2178,1221,2170,1203,2138,1194,2123,1197,2128,1232,2191,1206,2143,}, 
	{1374,2443,1362,2422,1351,2401,1340,2382,1333,2370,1318,2343,1308,2326,1295,2302,1287,2287,1280,2275,1268,2255,1261,2243,1255,2231,1253,2228,1249,2220,1221,2170,1249,2220,1207,2146,1203,2138,1198,2130,1193,2120,1194,2122,1238,2202,1227,2182,}, 
	{1367,2430,1358,2414,1344,2389,1335,2374,1326,2357,1312,2332,1300,2312,1291,2294,1284,2282,1273,2263,1263,2246,1254,2230,1268,2254,1240,2204,1255,2231,1221,2171,1215,2161,1212,2155,1203,2139,1202,2137,1199,2131,1193,2121,1243,2210,1241,2207,}, 
	{1361,2420,1353,2405,1347,2395,1335,2373,1327,2359,1315,2338,1298,2308,1291,2295,1288,2289,1274,2266,1263,2245,1258,2236,1246,2216,1246,2216,1239,2203,1257,2235,1229,2186,1249,2220,1262,2244,1211,2153,1208,2147,1200,2133,1253,2228,1250,2221,}, 
	{1358,2415,1350,2401,1339,2381,1330,2365,1325,2356,1312,2332,1303,2316,1291,2296,1281,2278,1275,2266,1262,2244,1254,2229,1255,2232,1248,2219,1245,2213,1236,2197,1230,2186,1227,2181,1219,2168,1217,2163,1210,2151,1210,2151,1261,2241,1231,2188,}, 
	{1355,2409,1357,2413,1349,2399,1333,2370,1325,2356,1311,2331,1302,2315,1292,2297,1287,2287,1275,2266,1265,2250,1256,2233,1301,2313,1261,2241,1267,2253,1250,2223,1242,2209,1238,2201,1232,2191,1231,2188,1225,2177,1219,2167,1270,2257,1260,2240,}, 
	{1352,2403,1353,2405,1343,2388,1333,2370,1322,2350,1312,2332,1300,2311,1291,2295,1288,2290,1277,2270,1267,2253,1257,2235,1290,2293,1272,2262,1265,2249,1255,2232,1251,2224,1246,2215,1239,2202,1239,2204,1232,2190,1230,2186,1282,2280,1267,2252,}, 
	{1353,2405,1351,2402,1342,2386,1333,2370,1322,2350,1311,2331,1301,2313,1294,2300,1286,2285,1276,2269,1269,2256,1257,2234,1303,2316,1294,2301,1280,2276,1272,2261,1303,2317,1260,2239,1254,2229,1253,2227,1244,2212,1239,2202,1291,2296,1303,2317,}, 
	{1350,2399,1346,2393,1336,2376,1328,2362,1318,2343,1304,2319,1293,2298,1286,2285,1278,2272,1269,2256,1259,2238,1249,2220,1326,2357,1302,2315,1287,2287,1281,2278,1271,2259,1266,2251,1260,2241,1257,2235,1255,2231,1252,2225,1304,2318,1347,2394,}, 
	{1315,2338,1309,2326,1297,2305,1286,2286,1273,2263,1260,2239,1247,2217,1240,2205,1231,2189,1215,2159,1205,2142,1191,2118,1803,3205,1519,2700,1436,2554,1406,2499,1385,2463,1370,2435,1360,2418,1356,2411,1349,2399,1349,2399,1413,2512,1497,2661,}, 
};


#define TX_NUM_TPK  (13)
#define RX_NUM_TPK  (24)
#define DiagonalUpperLimit_TPK  (1100)
#define DiagonalLowerLimit_TPK  (900)

const int16_t raw_cap_data_tpk[TX_NUM_TPK][RX_NUM_TPK*2] =
{
	{1476,2625,1473,2619,1418,2522,1407,2501,1398,2485,1390,2471,1382,2456,1374,2442,1368,2432,1366,2428,1373,2440,1458,2593,1147,2040,1143,2032,1135,2018,1126,2002,1121,1992,1114,1980,1107,1968,1127,2004,1122,1995,1101,1958,1164,2069,1046,1859,}, 
	{1441,2562,1413,2512,1389,2469,1372,2438,1358,2414,1346,2393,1335,2372,1321,2349,1312,2333,1304,2318,1294,2301,1292,2297,1224,2176,1221,2171,1214,2158,1207,2145,1214,2158,1225,2178,1192,2119,1210,2151,1202,2137,1183,2104,1248,2219,1156,2055,}, 
	{1417,2519,1402,2492,1381,2455,1367,2429,1354,2407,1343,2387,1330,2365,1316,2340,1306,2322,1296,2303,1284,2283,1276,2269,1230,2187,1227,2182,1220,2169,1213,2156,1208,2147,1205,2142,1198,2129,1222,2173,1211,2154,1190,2115,1254,2229,1122,1994,}, 
	{1402,2493,1390,2471,1376,2447,1365,2426,1353,2405,1342,2386,1329,2363,1314,2336,1303,2316,1292,2297,1280,2276,1272,2261,1234,2194,1231,2189,1225,2179,1222,2173,1233,2192,1227,2182,1230,2186,1208,2148,1208,2147,1195,2124,1252,2226,1113,1978,}, 
	{1393,2477,1385,2462,1374,2442,1363,2423,1354,2407,1343,2388,1329,2363,1314,2336,1302,2314,1290,2294,1278,2272,1268,2255,1256,2232,1257,2234,1250,2222,1259,2238,1219,2168,1215,2159,1212,2155,1212,2155,1205,2143,1202,2137,1252,2226,1133,2014,}, 
	{1388,2468,1382,2457,1372,2439,1362,2422,1353,2405,1342,2385,1328,2361,1312,2333,1300,2311,1288,2289,1275,2267,1265,2248,1270,2257,1265,2249,1238,2201,1230,2187,1227,2182,1223,2174,1220,2168,1221,2170,1214,2159,1211,2152,1260,2241,1138,2023,}, 
	{1387,2466,1384,2460,1374,2442,1364,2425,1354,2408,1343,2387,1329,2363,1314,2335,1301,2312,1288,2290,1275,2267,1264,2248,1252,2225,1249,2220,1244,2211,1238,2202,1258,2237,1251,2224,1275,2267,1225,2179,1219,2167,1215,2159,1264,2247,1126,2002,}, 
	{1388,2468,1385,2462,1375,2444,1364,2425,1354,2408,1343,2388,1330,2364,1315,2337,1302,2314,1290,2294,1278,2271,1267,2253,1262,2243,1262,2243,1263,2245,1285,2284,1247,2218,1245,2213,1243,2210,1242,2207,1236,2198,1232,2190,1284,2282,1139,2025,}, 
	{1387,2466,1384,2460,1373,2441,1362,2422,1351,2402,1339,2380,1326,2357,1311,2330,1298,2308,1288,2289,1275,2267,1264,2246,1269,2257,1266,2251,1260,2240,1256,2232,1253,2228,1250,2221,1247,2216,1248,2219,1243,2210,1240,2204,1292,2296,1160,2063,}, 
	{1385,2462,1382,2456,1371,2437,1360,2417,1349,2398,1337,2377,1325,2355,1311,2330,1299,2310,1290,2293,1278,2272,1267,2252,1329,2362,1299,2309,1282,2279,1272,2261,1267,2252,1261,2242,1258,2237,1259,2238,1253,2228,1250,2223,1303,2317,1166,2073,}, 
	{1385,2461,1378,2449,1365,2426,1352,2404,1341,2384,1329,2363,1317,2341,1303,2316,1293,2298,1283,2281,1272,2262,1261,2242,1318,2343,1302,2314,1290,2294,1281,2277,1275,2267,1269,2256,1266,2250,1266,2250,1260,2240,1258,2236,1313,2334,1185,2106,}, 
	{1391,2473,1381,2456,1367,2430,1352,2403,1339,2380,1327,2359,1314,2336,1300,2311,1290,2293,1280,2276,1269,2256,1258,2236,1414,2514,1331,2366,1317,2341,1300,2311,1292,2297,1285,2284,1280,2276,1281,2276,1277,2270,1277,2270,1339,2380,1264,2246,}, 
	{1352,2403,1335,2373,1315,2337,1295,2303,1279,2273,1263,2246,1249,2220,1232,2190,1219,2168,1208,2147,1194,2123,1181,2099,1732,3079,1554,2762,1481,2632,1436,2553,1407,2502,1386,2464,1372,2440,1369,2433,1362,2421,1364,2425,1436,2553,1418,2520,}, 
};


#endif


