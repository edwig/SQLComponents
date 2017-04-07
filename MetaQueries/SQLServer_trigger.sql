create trigger dbo.test_number_check on dbo.test_number
for update
as
begin
   declare @id int;
   declare @field1 int;
   declare @field2 real;

   select @id=i.id from inserted i;
   select @field1=i.field1 from inserted i;
   select @field2=i.field2 from inserted i;

   update test_number
	   set field3 = @field1 + @field2
    where id = @id;
end;


insert into test_number values (1,1,1,1);
update test_number
   set field1 = 4
      ,field2 = 6
where id = 1;

select * from test_number;

