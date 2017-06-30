# Wireless-sensor-network-project

Event	detection	in	a	fully	distributed	wireless	sensor	network - WSN

WSN	Description: 
- The	wireless	sensor	network	comprises	60 nodes.	These	nodes	are	arranged	in a	4	x	15 (rectangular-shaped) grid.	 Distance	between	the	adjacent	nodes	is	kept	as	such	that	these	nodes	can	wirelessly communicate. The	adjacent	nodes	can	exchange	data	through	unicast and	broadcast modes	of	communications.	
- Communication	between	non-adjacent	nodes	is	not	possible.	
- Every node	in	the	WSN	can	independently	exchange	data	with	the	base-station.	Basestation in	a	WSN is	a	centrally	located node that	is	entrusted	with	the	task of	gathering	data	from	all	other	nodes	in	the	WSN.
- Event	Detection Criterion: In	order	for	an	event	to	be	recorded	by	the	WSN,	at	least	four nearest nodes,	to	a	reference	node, must	simultaneously	report	their	activations	to	the	base-station (explanatory	figure	in	the	end).	The	base	station	then	collects	all	the	event	reports	and	writes these to	its	log	file.

Project goal: 
Develop	a	MPI	code	that	simulates	the	operation	of	this WSN in	an efficient	manner.	The	criterion	for	measuring	efficiency	in this	exercise is	in	finding	the	communication	scheme	that	minimises	the	messages	to	the	base-station	whilst satisfying the	WSN’s	event	detection	criterion	(stated	above).
Hints:
1. Assume	that	a	set	of	MPI	tasks	(processes) represents	the WSN and	each	MPI	task can	represent a	WSN	node.
2. To simulate	random	occurrences	of	events	within	the	WSN,	each	WSN	node may	be	provisioned	with	an	independent	random	number generator with	the	condition	that	four nearest nodes must	produce	a	random	number	above	some	threshold,	at	the	
sampling	time,	to	constitute	an	event.
3. Write	the	key	performance	metrics e.g.	the	simulation	time,	number	of	events detected,	number	of	messages/event	with	senders’ adjacency information/addresses,	total	number	of	messages	(for	this	simulation),	to	an	output file.	Doing	so may	assist	in	proving	the	correctness	and efficiency	of	your implementation.

Build:
- compile using makefile: make
- to run, use make file with command: make run (the program only accepts 61 processes to stimulate the operation)
