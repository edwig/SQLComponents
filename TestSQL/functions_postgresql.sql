create or replace function public.testmul2(x int)
returns int as $$
begin
  return 2 * x;
end;
$$ language plpgsql;


create or replace function public.testins(x varchar(200))
returns int as $$
begin
  insert into public.test_record(name) values (x);
  return 1;
end;
$$ language plpgsql;


drop function public.multinout(varchar(200));

create or replace function public.multinout(x varchar(200))
returns record as $$
declare
   y varchar(200);
   resul record;
begin
   y := x || x;
   insert into public.test_record(name) values (y);
   select 77.88,y into resul;
   return resul;
end;   
$$ language plpgsql;

create or replace function public.getdecimal(x varchar(40))
returns decimal(18,2) as $$
declare
   res decimal(18,2);
begin
   res := x;
   return res;
end;
$$ language plpgsql;

drop function public.multinout2(varchar(200));

create or replace function public.multinout2(x varchar(200))
returns table (one decimal(18,2),two varchar(200)) as $$
declare
   y varchar(200);
   resul record;
begin
   y := x || x;
   insert into public.test_record(name) values (y);
   return query
   select 77.88,y;
end;   
$$ language plpgsql;

create table public.test_record
(
    name varchar(200)
);

