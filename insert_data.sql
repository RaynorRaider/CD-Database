-- Delete existing data
delete from tracks;
delete from cd;
delete from artist;

-- Data inserts

insert into artist(id,name) values(1, 'Pink Floyd');
insert into artist(id,name) values(2, 'Genesis');
insert into artist(id,name) values(3, 'Einaudi');
insert into artist(id,name) values(4, 'Melanie C');

insert into cd(id, title, artist_id, catalogue) values(1, 'Dark Side of the Moon', 1, 'B000024D4P');
insert into cd(id, title, artist_id, catalogue) values(2, 'Wish You Were Here', 1, 'B000024D4S');
insert into cd(id, title, artist_id, catalogue) values(3, 'A Trick of the Tail', 2, 'B000024EXM');
insert into cd(id, title, artist_id, catalogue) values(4, 'Selling England By the Pound', 2, 'B000024E9M');
insert into cd(id, title, artist_id, catalogue) values(5, 'I Giorni', 3, 'B000071WEV');
insert into cd(id, title, artist_id, catalogue) values(6, 'Northern Star', 4, 'B00004YMST');


insert into tracks(cd_id, track_id, title) values(1, 1, 'Speak to me');
insert into tracks(cd_id, track_id, title) values(1, 2, 'Breathe');

insert into tracks(cd_id, track_id, title) values(2, 1, 'Shine on you crazy diamond');
insert into tracks(cd_id, track_id, title) values(2, 2, 'Welcome to the machine');
insert into tracks(cd_id, track_id, title) values(2, 3, 'Have a cigar');
insert into tracks(cd_id, track_id, title) values(2, 4, 'Wish you were here');
insert into tracks(cd_id, track_id, title) values(2, 5, 'Shine on you crazy diamond pt.2');


insert into tracks(cd_id, track_id, title) values(5, 1, 'Melodia Africana (part 1)');
insert into tracks(cd_id, track_id, title) values(5, 2, 'I due fiumi');
insert into tracks(cd_id, track_id, title) values(5, 3, 'In un\'altra vita');


insert into tracks(cd_id, track_id, title) values(6, 11, 'Closer');

insert into tracks(cd_id, track_id, title) values(6, 12, 'Feel The Sun');

















