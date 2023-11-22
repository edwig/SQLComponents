-- TEST PROCEDURE for named parameters
-- Can be called with all or just a subset of parameters
-- RDBMS: SQL-Server
-- Module: SQLComponents
-- Project: https://github.com/edwig/SQLComponents
--
create or alter procedure bignumber 
 @numbers int = 0
,@tennumbers int = 0
,@hunderdnumbers int = 0
,@thousandnumbers int = 0
,@tenthousand int = 0
,@hunderdthousand int = 0
,@milions int = 0
as 
begin
declare @amount int;
set @amount = @numbers 
            + 10 * @tennumbers 
			+ 100 * @hunderdnumbers
			+ 1000 * @thousandnumbers
			+ 10000 * @tenthousand
			+ 100000 * @hunderdthousand
			+ 1000000 * @milions;
return @amount;
end;


-- Test with all parameters
declare @total int;
exec @total = bignumber 9,3,2,1,default,6,8
select @total;


-- Test with named parameters only
declare @total2 int;
exec @total2 = bignumber @tenthousand = 6,@tennumbers = 4
select @total2;

