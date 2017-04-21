hdrs = ipc.h
opts = -g -c
p_src = prov.c ipc.c
p_obj = prov.o ipc.o
s1_src = smoke1.c ipc.c
s1_obj = smoke1.o ipc.o
s2_src = smoke2.c ipc.c
s2_obj = smoke2.o ipc.o
s3_src = smoke3.c ipc.c
s3_obj = smoke3.o ipc.o
all:	prov smoke1 smoke2 smoke3
smoke1:		$(s1_obj)
			gcc $(s1_obj) -o smoke1
smoke1.o:	$(s1_src) $(hdrs)
			gcc $(opts) $(s1_src)
smoke2:		$(s2_obj)
			gcc $(s2_obj) -o smoke2
smoke2.o:	$(s2_src) $(hdrs)
			gcc $(opts) $(s2_src)
smoke3:		$(s3_obj)
			gcc $(s3_obj) -o smoke3
smoke3.o:	$(s3_src) $(hdrs)
			gcc $(opts) $(s3_src)
prov:	 	$(p_obj)
			gcc $(p_obj) -o prov
prov.o:		$(p_src) $(hdrs)
			gcc $(opts) $(p_src)
clean:
			rm smoke1 smoke2 smoke3 prov *.o
