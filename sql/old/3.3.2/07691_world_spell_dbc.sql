-- Add serveurside spells place holders for future development
DELETE FROM `spell_dbc` WHERE `Id` IN (42876,44987,48803,68496,72958,32780,45453);
INSERT INTO `spell_dbc` (`Id`,`Comment`) VALUES
(42876, 'Quest 9275 reward serveurside spell'),
(44987, 'Quest 11521 reward serveurside spell'),
(48803, 'Quest 12214 reward serveurside spell'),
(68496, 'Item_template serveurside spell'),
(72958, 'Item_template serveurside spell'),
(32780, 'Quest 10040 reward serveurside spell'),
(45453, 'Quest 11587 reward serveurside spell');
