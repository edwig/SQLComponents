-- FUnctions for MS-SQL Server

DROP TABLE dbo.testrecord;

CREATE TABLE dbo.testrecord
(
   name VARCHAR(400)
);


drop procedure dbo.testmul2;
go

CREATE procedure dbo.testmul2( @p_num INTEGER)
AS
  BEGIN
    return @p_num * 2;
  END
go

drop procedure dbo.getdecimal;
go

create function dbo.getdecimal(@p_num varchar(200))
returns decimal(18,2)
as begin
  declare @v_num decimal(18,2);
  set @v_num = @p_num;
  return @v_num;
end
go

exec dbo.getdecimal '44.99';
go

drop procedure dbo.testins;
go

CREATE PROCEDURE dbo.testins(@p_text VARCHAR(200))
  AS
BEGIN
  declare @v_dyn varchar(500);
  INSERT INTO dbo.testrecord(name) VALUES (@p_text );
END
go

exec dbo.testins 'Testing';

delete from dbo.testrecord;
select * from dbo.testrecord;

drop procedure dbo.multinout;
go

CREATE procedure dbo.multinout( @p_text VARCHAR(200), @v_num decimal(18,2) OUTPUT, @p_total VARCHAR(200) OUTPUT)
As
  DECLARE @v_plus VARCHAR(200);
  DECLARE @v_dyn  varchar(500);
  SET  @v_plus = @p_text + @p_text;
  SET @p_total = substring(@v_plus,1,100);
  INSERT INTO dbo.testrecord (name) VALUES (@p_total);
  set @v_num = 77.88;
  return 2;
RETURN
go

declare @v_num decimal(18,2);
declare @p_total varchar(200);
declare @status int;
exec @status = dbo.multinout 'APE', @v_num OUT, @p_total OUT;
select @v_num   as theNumber
      ,@p_total as theString
	  ,@status  as theStatus;
go

declare @num decimal(18,2);
declare @p_num varchar(200);
set @p_num = '77.88';
exec @num = dbo.getdecimal @p_num;
select @p_num;
go

SELECT * FROM testrecord;
DELETE FROM testrecord;