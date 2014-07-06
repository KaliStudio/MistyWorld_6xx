-- Add serveurside spells place holders for future development
DELETE FROM `spell_dbc` WHERE `Id` IN (25347,45315,43236,43459,43499,44275,64689,50574);
INSERT INTO `spell_dbc` (`Id`,`Comment`) VALUES
(25347, 'Item_template serveurside spell'),
(45315, 'Quest 11566 reward serveurside spell'),
(43236, 'Quest 11288 reward serveurside spell'),
(43459, 'Quest 11332 reward serveurside spell'),
(43499, 'Quest 11250 reward serveurside spell'),
(44275, 'Quest 11432 reward serveurside spell'),
(64689, 'Quest 13854 and 13862 reward serveurside spell'),
(50574, 'Quest 12597 reward serveurside spell');
