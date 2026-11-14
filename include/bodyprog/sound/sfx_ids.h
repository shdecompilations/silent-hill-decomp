#ifndef _BODYPROG_SOUND_SFXIDS_H
#define _BODYPROG_SOUND_SFXIDS_H

// TODO: Rename to `SfxId`. There's a name clash with an SFX struct.
typedef enum _SfxId
{
    Sfx_Base = 1280,                  // Base SFX (not valid in itself).

    Sfx_MenuStartGame = Sfx_Base + 1, // `SND/FIRST.VAB`

    Sfx_Unk1283 = Sfx_Base + 3,
    Sfx_Unk1284 = Sfx_Base + 4,
    Sfx_Unk1285 = Sfx_Base + 5,
    Sfx_Unk1286 = Sfx_Base + 6,
    Sfx_Unk1287 = Sfx_Base + 7,
    Sfx_Unk1288 = Sfx_Base + 8,
    Sfx_Unk1289 = Sfx_Base + 9,
    Sfx_Unk1290 = Sfx_Base + 10,
    Sfx_Unk1291 = Sfx_Base + 11,
    Sfx_Unk1292 = Sfx_Base + 12,
    Sfx_Unk1293 = Sfx_Base + 13,
    Sfx_Unk1294 = Sfx_Base + 14,
    Sfx_Unk1295 = Sfx_Base + 15,
    Sfx_Unk1296 = Sfx_Base + 16,
    Sfx_Unk1297 = Sfx_Base + 17,
    Sfx_Unk1298 = Sfx_Base + 18,
    Sfx_Unk1299 = Sfx_Base + 19,
    Sfx_Unk1300 = Sfx_Base + 20,
    Sfx_Unk1301 = Sfx_Base + 21,
    Sfx_Unk1302 = Sfx_Base + 22,
    Sfx_Unk1303 = Sfx_Base + 23,

    Sfx_MenuError        = Sfx_Base + 0x18, // 1304, `1ST/BASE.VAB` onward, but loaded out of order?
    Sfx_MenuMove         = Sfx_Base + 0x19, // 1305
    Sfx_MenuCancel       = Sfx_Base + 0x1A, // 1306
    Sfx_MenuConfirm      = Sfx_Base + 0x1B, // 1307
    Sfx_MenuMap          = Sfx_Base + 0x1C, // 1308
    Sfx_DoorOpen0        = Sfx_Base + 0x1D, // 1309
    Sfx_DoorClose0       = Sfx_Base + 0x1E, // 1310
    Sfx_FootstepConcrete = Sfx_Base + 0x1F, // 1311
    Sfx_FootstepGrass    = Sfx_Base + 0x20, // 1312
    Sfx_Unk1313          = 1313, // Harry footstep for unknown material.
    Sfx_Stumble0         = Sfx_Base + 0x22, // 1314

    Sfx_Unk1316               = 1316,
    Sfx_HarryFallGasp         = 1317,
    Sfx_Unk1318               = 1318,
    Sfx_Unk1319               = 1319,
    Sfx_HarryHeavyBreath      = Sfx_Base + 0x28, // 1320
    Sfx_RadioInterferenceLoop = Sfx_Base + 0x29, // 1321
    Sfx_RadioStaticLoop       = Sfx_Base + 0x2A, // 1322
    Sfx_DoorOpenM2S01         = 1323,            // } Pair.
    Sfx_DoorCloseM2S01        = 1324,            // }
    Sfx_MenuHealthItemUse     = 1325,
    Sfx_Unk1326               = 1326,
    Sfx_Unk1327               = 1327,
    Sfx_Unk1328               = 1328,
    Sfx_Unk1329               = 1329,

    Sfx_FootstepUnk1330       = 1330, // Harry footstep for unknown material.
    Sfx_FootstepUnk1331       = 1331, // Harry footstep for unknown material.
    Sfx_FootstepMetal = Sfx_Base + 0x34, // 1332
    Sfx_Stumble1      = Sfx_Base + 0x35, // 1333
    Sfx_DoorJammed    = Sfx_Base + 0x36, // 1334
    Sfx_UseKey        = Sfx_Base + 0x37, // 1335
    Sfx_DoorOpenM7S01         = 1336,            // } Pair.
    Sfx_DoorCloseM7S01        = 1337,            // }
    Sfx_DoorOpenUnk1338       = 1338,            // } Pair.
    Sfx_DoorCloseUnk1339      = 1339,            // }
    Sfx_DoorOpenUnk1340       = 1340,            // } Pair.
    Sfx_DoorCloseUnk1341      = 1341,            // }
    Sfx_DoorUnlocked  = Sfx_Base + 0x3E, // 1342
    Sfx_DoorPuzzleUnlocked       = 1343,
    Sfx_DoorLocked    = Sfx_Base + 0x40, // 1344

    Sfx_FootstepMetal1 = 1346,

    Sfx_DoorLocked1 = 1349,
    Sfx_DoorLocked2 = 1350,
    Sfx_DoorOpenUnk1351  = 1351, // } Pair.
    Sfx_DoorCloseUnk1352 = 1352, // }
    Sfx_CherylFootstep = 1353,
    Sfx_Unk1354 = 1354,
    Sfx_Unk1355 = 1355,
    Sfx_Unk1356 = 1356,
    Sfx_Unk1357 = 1357,
    Sfx_Unk1358 = 1358,
    Sfx_Unk1359 = 1359,
    Sfx_Unk1360 = 1360,
    Sfx_Unk1361 = 1361,

    Sfx_Unk1363 = 1363, // } Stalker sounds.
    Sfx_Unk1364 = 1364, // }
    Sfx_Unk1365 = 1365, // }
    Sfx_Unk1366 = 1366, // }

    Sfx_Unk1368 = 1368,
    Sfx_Unk1369 = 1369,
    Sfx_Unk1370 = 1370,

    Sfx_Unk1373 = 1373,
    Sfx_Unk1374 = 1374,
    Sfx_Unk1375 = 1375,

    Sfx_Unk1384 = 1384,
    Sfx_Unk1385 = 1385,
    Sfx_Unk1386 = 1386,
    Sfx_Unk1387 = 1387,
    Sfx_Unk1388 = 1388, // Harry footstep for unknown material.
    Sfx_Unk1389 = 1389,

    Sfx_Unk1390 = 1390,
    Sfx_Unk1391 = 1391,
    Sfx_Unk1392 = 1392,
    Sfx_Unk1393 = 1393,
    Sfx_Unk1394 = 1394,

    Sfx_Unk1396 = 1396, // } Pair.
    Sfx_Unk1397 = 1397, // }
    Sfx_Unk1398 = 1398, // } Pair.
    Sfx_Unk1399 = 1399, // }
    Sfx_Unk1400 = 1400,
    Sfx_Unk1401 = 1401,
    Sfx_Unk1402 = 1402,
    Sfx_Unk1403 = 1403,
    Sfx_Unk1404 = 1404,
    Sfx_RomperFootstep = 1405,
    Sfx_Unk1406 = 1406, // Groaner.

    Sfx_Unk1409 = 1409, // Groaner.
    Sfx_Unk1410 = 1410, // Groaner.
    Sfx_Unk1411 = 1411, // Groaner.

    Sfx_GroanerFootstep = 1413,
    Sfx_Unk1414 = 1414, // Groaner.

    Sfx_Unk1416 = 1416, // Place item in receptable?
    Sfx_Unk1417 = 1417,

    Sfx_Unk1418 = 1418,
    Sfx_Unk1419 = 1419,
    Sfx_Unk1420 = 1420,
    Sfx_Unk1421 = 1421,
    Sfx_Unk1422 = 1422,
    Sfx_Unk1423 = 1423,
    Sfx_Unk1424 = 1424,
    Sfx_Unk1425 = 1425, // Creeper hit?
    Sfx_Unk1426 = 1426, // Creeper chirp.
    Sfx_Unk1427 = 1427, // Creeper chirp.

    Sfx_Unk1429 = 1429,
    Sfx_Unk1430 = 1430,
    Sfx_Unk1431 = 1431, // } Pair.
    Sfx_Unk1432 = 1432, // }
    Sfx_Unk1433 = 1433,
    Sfx_Unk1434 = 1434,
    Sfx_Unk1435 = 1435,
    Sfx_Unk1436 = 1436,
    Sfx_Unk1437 = 1437,
    Sfx_Unk1438 = 1438, // } Piano keys.
    Sfx_Unk1439 = 1439, // }
    Sfx_Unk1440 = 1440, // }
    Sfx_Unk1441 = 1441, // }
    Sfx_Unk1442 = 1442, // }
    Sfx_Unk1443 = 1443, // }
    Sfx_Unk1444 = 1444, // }
    Sfx_Unk1445 = 1445, // }
    Sfx_Unk1446 = 1446,
    Sfx_CatMeow = 1447,
    Sfx_Unk1448 = 1448,
    Sfx_Unk1449 = 1449,
    Sfx_Unk1450 = 1450,
    Sfx_Unk1451 = 1451,
    Sfx_Unk1452 = 1452,
    Sfx_Unk1453 = 1453,
    Sfx_Unk1454 = 1454,
    Sfx_Unk1455 = 1455,
    Sfx_Unk1456 = 1456,
    Sfx_Unk1457 = 1457,
    Sfx_Unk1458 = 1458, // } Pair.
    Sfx_Unk1459 = 1459, // }
    Sfx_Unk1460 = 1460,
    Sfx_Unk1461 = 1461,
    Sfx_Unk1462 = 1462,
    Sfx_Unk1463 = 1463,
    Sfx_Unk1464 = 1464,
    Sfx_Unk1465 = 1465,
    Sfx_Unk1466 = 1466,
    Sfx_Unk1467 = 1467,
    Sfx_Unk1468 = 1468,

    Sfx_Unk1471 = 1471,
    Sfx_Unk1472 = 1472,
    Sfx_Unk1473 = 1473,
    Sfx_Unk1474 = 1474,
    Sfx_Unk1475 = 1475,
    Sfx_Unk1476 = 1476,
    Sfx_Unk1477 = 1477,
    Sfx_Unk1478 = 1478,
    Sfx_Unk1479 = 1479,
    Sfx_Unk1480 = 1480,
    Sfx_Unk1481 = 1481,
    Sfx_Unk1482 = 1482,
    Sfx_Unk1483 = 1483,
    Sfx_Unk1484 = 1484,
    Sfx_Unk1485 = 1485,
    Sfx_Unk1486 = 1486,
    Sfx_Unk1487 = 1487,
    Sfx_Unk1488 = 1488,
    Sfx_Unk1489 = 1489,
    Sfx_Unk1490 = 1490, // } Pair.
    Sfx_Unk1491 = 1491, // }
    Sfx_Unk1492 = 1492,
    Sfx_Unk1493 = 1493,
    Sfx_Unk1494 = 1494,
    Sfx_Unk1495 = 1495,
    Sfx_Unk1496 = 1496,
    Sfx_Unk1497 = 1497,
    Sfx_Unk1498 = 1498,
    Sfx_Unk1499 = 1499,
    Sfx_Unk1500 = 1500,
    Sfx_Unk1501 = 1501, // } Played when opening the load screen.
    Sfx_Unk1502 = 1502, // }
    Sfx_Unk1503 = 1503,
    Sfx_Unk1504 = 1504, // } Pair.
    Sfx_Unk1505 = 1505, // }

    Sfx_Unk1520 = 1520,
    Sfx_Unk1521 = 1521,
    Sfx_Unk1522 = 1522,
    Sfx_Unk1523 = 1523,
    Sfx_Unk1524 = 1524,

    Sfx_Unk1525 = 1525, // Bloodsucker.
    Sfx_Unk1526 = 1526,
    Sfx_Unk1527 = 1527,
    Sfx_Unk1528 = 1528,
    Sfx_Unk1529 = 1529,
    Sfx_Unk1530 = 1530,
    Sfx_Unk1531 = 1531,
    Sfx_Unk1532 = 1532,
    Sfx_Unk1533 = 1533,
    Sfx_Unk1534 = 1534,
    Sfx_Unk1535 = 1535,
    Sfx_Unk1536 = 1536,
    Sfx_Unk1537 = 1537,
    Sfx_Unk1538 = 1538,
    Sfx_Unk1539 = 1539,
    Sfx_Unk1540 = 1540,
    Sfx_Unk1541 = 1541, // Door jammed alt.
    Sfx_Unk1542 = 1542,
    Sfx_Unk1543 = 1543, // Harry footstep for unknown material.

    Sfx_Unk1552 = 1552,
    Sfx_Unk1553 = 1553,
    Sfx_Unk1554 = 1554,
    Sfx_Unk1555 = 1555,
    Sfx_Unk1556 = 1556,
    Sfx_Unk1557 = 1557, // Harry footstep for unknown material.

    Sfx_Unk1559 = 1559,

    Sfx_Unk1563 = 1563,
    Sfx_Unk1564 = 1564,
    Sfx_Unk1565 = 1565,

    Sfx_Unk1567 = 1567,
    Sfx_Unk1568 = 1568,
    Sfx_Unk1569 = 1569,
    Sfx_Unk1570 = 1570,
    Sfx_Unk1571 = 1571,
    Sfx_Unk1572 = 1572,
    Sfx_Unk1573 = 1573,
    Sfx_Unk1574 = 1574,
    Sfx_Unk1575 = 1575,
    Sfx_Unk1576 = 1576,
    Sfx_Unk1577 = 1577, // } Hanged Scratcher.
    Sfx_Unk1578 = 1578, // }
    Sfx_Unk1579 = 1579, // }
    Sfx_Unk1580 = 1580, // }
    Sfx_Unk1581 = 1581, // }
    Sfx_Unk1582 = 1582, // }
    Sfx_Unk1583 = 1583, // }
    Sfx_Unk1584 = 1584, // }
    Sfx_Unk1585 = 1585,
    Sfx_Unk1586 = 1586,
    Sfx_Unk1587 = 1587,
    Sfx_Unk1588 = 1588,
    Sfx_Unk1589 = 1589,
    Sfx_Unk1590 = 1590,
    Sfx_Unk1591 = 1591,
    Sfx_Unk1592 = 1592,
    Sfx_Unk1593 = 1593,
    Sfx_Unk1594 = 1594,
    Sfx_Unk1595 = 1595,
    Sfx_Unk1596 = 1596,
    Sfx_Unk1597 = 1597,
    Sfx_Unk1598 = 1598,
    Sfx_Unk1599 = 1599,
    Sfx_Unk1600 = 1600,
    Sfx_Unk1601 = 1601, // } Pair.
    Sfx_Unk1602 = 1602, // }

    Sfx_Unk1604 = 1604, // } Pair.
    Sfx_Unk1605 = 1605, // }
    Sfx_Unk1606 = 1606,
    Sfx_Unk1607 = 1607,
    Sfx_Unk1608 = 1608,
    Sfx_Unk1609 = 1609, // } Pair.
    Sfx_Unk1610 = 1610, // }
    Sfx_Unk1611 = 1611,
    Sfx_Unk1612 = 1612,
    Sfx_Unk1613 = 1613,
    Sfx_Unk1614 = 1614,
    Sfx_Unk1615 = 1615,
    Sfx_Unk1616 = 1616,
    Sfx_Unk1617 = 1617,
    Sfx_Unk1618 = 1618,
    Sfx_Unk1619 = 1619,
    Sfx_Unk1620 = 1620,
    Sfx_Unk1621 = 1621,
    Sfx_Unk1622 = 1622,
    Sfx_Unk1623 = 1623,
    Sfx_Unk1624 = 1624,
    Sfx_Unk1625 = 1625,
    Sfx_Unk1626 = 1626,
    Sfx_Unk1627 = 1627,
    Sfx_Unk1628 = 1628,
    Sfx_Unk1629 = 1629,
    Sfx_Unk1630 = 1630,
    Sfx_Unk1631 = 1631,
    Sfx_Unk1632 = 1632,
    Sfx_Unk1633 = 1633,
    Sfx_Unk1634 = 1634,
    Sfx_Unk1635 = 1635,
    Sfx_Unk1636 = 1636,
    Sfx_Unk1637 = 1637,
    Sfx_Unk1638 = 1638,
    Sfx_Unk1639 = 1639,
    Sfx_Unk1640 = 1640,

    Sfx_Unk1642 = 1642,
    Sfx_Unk1643 = 1643,
    Sfx_Unk1644 = 1644,
    Sfx_Unk1645 = 1645,
    Sfx_Unk1646 = 1646,
    Sfx_Unk1647 = 1647,
    Sfx_Unk1648 = 1648,
    Sfx_Unk1649 = 1649,
    Sfx_Unk1650 = 1650,
    Sfx_Unk1651 = 1651,
    Sfx_Unk1652 = 1652,
    Sfx_Unk1653 = 1653,
    Sfx_Unk1654 = 1654,
    Sfx_Unk1655 = 1655,
    Sfx_Unk1656 = 1656,
    Sfx_Unk1657 = 1657,
    Sfx_Unk1658 = 1658,

    Sfx_Unk1660 = 1660,
    Sfx_Unk1661 = 1661,
    Sfx_Unk1662 = 1662,
    Sfx_Unk1663 = 1663,
    Sfx_Unk1664 = 1664,
    Sfx_Unk1665 = 1665,
    Sfx_Unk1666 = 1666,
    Sfx_Unk1667 = 1667,
    Sfx_Unk1668 = 1668,
    Sfx_Unk1669 = 1669,
    Sfx_Unk1670 = 1670,
    Sfx_Unk1671 = 1671,
    Sfx_Unk1672 = 1672,
    Sfx_Unk1673 = 1673,
    Sfx_Unk1674 = 1674,
    Sfx_Unk1675 = 1675,

    Sfx_Unk1688 = 1688,
    Sfx_Unk1689 = 1689,
    Sfx_Unk1690 = 1690,

    Sfx_Unk1692 = 1692,

    Sfx_Unk1694 = 1694,
    Sfx_Unk1695 = 1695,

    Sfx_Unk1699 = 1699,

    Sfx_Unk1916 = 1916,

    /** Values starting from `4096` are XA files defined in `g_XaItemData` struct.
     * In both `Sd_XaAudioPlayTaskAdd` (called in `SD_Call`) and `Sd_XaPreLoadAudioTaskAdd`
     * (called in `Sd_XaPreLoadAudio`), when the command is assigned to
     * `g_Sd_AudioWork.xaAudioIdxCheck_2` or `g_Sd_AudioWork.xaAudioIdxCheck_4`, it is also masked with `0xFFF`.
     */

    Sfx_XaAudio12 = 4108,

    Sfx_XaAudio53 = 4149,
    Sfx_XaAudio54 = 4150,

    Sfx_XaAudio56 = 4152,

    Sfx_XaAudio562 = 4658,

    Sfx_XaAudio565 = 4661,

    Sfx_XaAudio568 = 4664,
    Sfx_XaAudio569 = 4665,

    Sfx_XaAudio596 = 4692,

    Sfx_XaAudio600 = 4696,

    Sfx_XaAudio602 = 4698,

    Sfx_XaAudio603 = 4699,

    Sfx_XaAudio606 = 4702,

    Sfx_XaAudio612 = 4708,

    Sfx_XaAudio635 = 4731,

    Sfx_XaAudio657 = 4753,

    Sfx_XaAudio723 = 4819,
    Sfx_XaAudio724 = 4820,
    Sfx_XaAudio725 = 4821,
    Sfx_XaAudio726 = 4822,

    Sfx_Hack = NO_VALUE, // @hack Force enum to be treated as `s32`.
} e_SfxId;

#endif
