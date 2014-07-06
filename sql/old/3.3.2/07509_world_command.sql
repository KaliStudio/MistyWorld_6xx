DELETE FROM `command` WHERE `name`='serveur shutdown';
INSERT INTO `command` (`name`,`security`,`help`) VALUES ('serveur shutdown','3','Syntax: .serveur shutdown #delay [#exit_code]\r\n\r\nShut the serveur down after #delay seconds. Use #exit_code or 0 as program exit code.');

