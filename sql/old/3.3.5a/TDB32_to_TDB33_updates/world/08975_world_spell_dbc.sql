-- Add serveurside spells place holders for future development
DELETE FROM `spell_dbc` WHERE `Id` IN (38406);
INSERT INTO `spell_dbc` (`Id`,`Comment`) VALUES
(38406, 'Quest 10721 RewSpellCast serveurside spell');
