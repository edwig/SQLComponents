-- create a table with all datatypes

DROP TABLE pronto.all_types;

CREATE TABLE pronto.all_types
(
   one 				CHAR(10)
	,two 				VARCHAR(200)
	,three 			int
	,four  			float
	,five  			double
	,six   			NUMBER(16,2)
	,seven 			DECIMAL(16,2) COMMENT 'seven of nine'
	,eight 			date
	,nine  			time
	,ten   			timestamp
	,eleven 		tinyint
	,twelve 		smallint
	,thirteen 	bigint
	,fourteen   number
	,fifteen    decimal
	,sixteen    TINYINT(1)
	,seventeen  SMALLINT(2)
	,eighteen   INT(8)
	,nineteen   BIGINT(16)
	,twenty     boolean
	,twentyone  tinytext
	,twentytwo  text
	,twentythree mediumtext
	,twentyfour  longtext
  ,twentyfive  INTEGER unsigned
  ,twentysix   DECIMAL(38,4)
  ,twentyseven BLOB
  ,twentyeight tinyblob
  ,twentynine  SET('A','B','C')
  ,thirty      enum('aap','noot','mies')
)

