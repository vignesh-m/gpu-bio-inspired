IFLAGS = -I./include

SRCS = src/graph.cu src/util.cu src/ant_colony.cu
OBJS = $(subst .cu,.o,$(SRCS))


all: test_aco_cu

%.o: %.cu
	nvcc $(IFLAGS) -c $< -o $@


test_aco_cu: $(OBJS) tests/test_aco.cu 
	nvcc $(IFLAGS) tests/test_aco.cu  $(OBJS) -o test_aco_cu

clean:
	rm -f error output test_aco_cu output2 error2 $(OBJS)
