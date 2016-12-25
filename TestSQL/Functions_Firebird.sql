set term ^;

create or alter function testmul2(x int)
returns int
as
  begin
    return 2*x;
  end;
^  

create or alter function testins(x varchar(200))
returns int
as 
  begin
    insert into test_record(name) values (:x);
    return 1;
  end;
^

create or alter procedure multinout(x varchar(200))
returns (y decimal(18,2),z varchar(200))
as 
  begin
    y = 77.88;
    z = :x || :x;
    insert into test_record (name) values (:z);
    suspend;
  end;
^

set term ;^
