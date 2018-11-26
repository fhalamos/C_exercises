#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define n_bins 1000

typedef struct{
	double x;
	double y;
	double z;
	double v_x;
	double v_y;
	double v_z;
	int state;
} Particle;

typedef struct{
	int age;
	char name[64];
	double gpa;
} Student;

int equal_words(char *str1, char* str2)
{
	if(strlen(str1)== strlen(str2) && strncmp(str1,str2,strlen(str2))==0)
		return 1;
	else
		return 0; 
}


Particle * get_particles()
{
	Particle* particles = (Particle *) malloc(10*sizeof(Particle));
	
	particles[0].x = 0.2;
	particles[0].y = 0.98;
	particles[0].z = 0.67;
	particles[0].v_x = 0.73;
	particles[0].v_y = 0.1;
	particles[0].v_z = 0.08;
	particles[0].state = 1;

	particles[1].x = 0.52;
	particles[1].y = 0.53;
	particles[1].z = 0.1;
	particles[1].v_x = 0.48;
	particles[1].v_y = 0.06;
	particles[1].v_z = 0.13;
	particles[1].state = 1;

	particles[2].x = 0.87;
	particles[2].y = 0.28;
	particles[2].z = 0.36;
	particles[2].v_x = 0.75;
	particles[2].v_y = 0.68;
	particles[2].v_z = 0.94;
	particles[2].state = 0;

	particles[3].x = 0.06;
	particles[3].y = 0.57;
	particles[3].z = 0.32;
	particles[3].v_x = 0.93;
	particles[3].v_y = 0.01;
	particles[3].v_z = 0.68;
	particles[3].state = 2;

	particles[4].x = 0.26;
	particles[4].y = 0.32;
	particles[4].z = 0.18;
	particles[4].v_x = 0.41;
	particles[4].v_y = 0.48;
	particles[4].v_z = 0.49;
	particles[4].state = 0;

	particles[5].x = 0.19;
	particles[5].y = 0.91;
	particles[5].z = 0.56;
	particles[5].v_x = 0.9;
	particles[5].v_y = 0.53;
	particles[5].v_z = 0.22;
	particles[5].state = 0;

	particles[6].x = 0.26;
	particles[6].y = 0.32;
	particles[6].z = 0.18;
	particles[6].v_x = 0.41;
	particles[6].v_y = 0.48;
	particles[6].v_z = 0.49;
	particles[6].state = 0;

	particles[7].x = 0.46;
	particles[7].y = 0.77;
	particles[7].z = 0.18;
	particles[7].v_x = 0.16;
	particles[7].v_y = 0.93;
	particles[7].v_z = 0.11;
	particles[7].state = 2;

	particles[8].x = 0.29;
	particles[8].y = 0.45;
	particles[8].z = 0.17;
	particles[8].v_x = 0.05;
	particles[8].v_y = 0.01;
	particles[8].v_z = 0.59;
	particles[8].state = 4;

	particles[9].x = 0.46;
	particles[9].y = 0.77;
	particles[9].z = 0.18;
	particles[9].v_x = 0.16;
	particles[9].v_y = 0.93;
	particles[9].v_z = 0.11;
	particles[9].state = 2;

	return particles;

}

Student * get_students()
{
	Student * students = (Student *) malloc (10* sizeof(Student));
	
	students[0].age = 23;
	students[0].gpa = 3.9;
	strcpy(students[0].name,"Isaac Newton");
	
	students[1].age = 25;
	students[1].gpa = 2.4;
	strcpy(students[1].name,"Albert Einstein");

	students[2].age = 29;
	students[2].gpa = 4;	
	strcpy(students[2].name,"Enrico Fermi");
	
	students[3].age = 20;
	students[3].gpa = 3.2;
	strcpy(students[3].name,"Johannes Kepler");
	
	students[4].age = 31;
	students[4].gpa = 3.6;
	strcpy(students[4].name,"Nicolaus Copernicus");
	
	students[5].age = 29;
	students[5].gpa = 4;
	strcpy(students[5].name,"Enrico Fermi");
	
	students[6].age = 34;
	students[6].gpa = 2.9;
	strcpy(students[6].name,"Galileo Galilei");
	
	students[7].age = 34;
	students[7].gpa = 2.9;
	strcpy(students[7].name,"Archimedes");
	
	students[8].age = 20;
	students[8].gpa = 3.2;
	strcpy(students[8].name,"Johannes Kepler");
	
	students[9].age = 29;
	students[9].gpa = 5;
	strcpy(students[9].name,"Enrico Fermi");

	return students;

}

//Based on bernstein_hash
unsigned int hash_particles(const void *key)
{
	unsigned int h = 5381;
	char* keyBytes = (char*)key;
 
	for( size_t i = 0; i < sizeof(Particle); i++)
		h = 33 * h + keyBytes[i];

	return h % n_bins;
}

unsigned int hash_students(const void* key)
{
	unsigned int h = 5381;

	Student * s = (Student*)key;

	//hash age
	for(size_t i = 0; i < sizeof(int); i++)
		h = 33 * h + ((char*)(&s->age))[i];

	//hash name
	for(size_t i = 0; i < strlen(s->name); i++)
		h = 33 * h + ((char*)(&s->name))[i];

	//hash gpa
	for(size_t i = 0; i < sizeof(double); i++)
		h = 33 * h + ((char*)(&s->gpa))[i];
	
	return h % n_bins;

}

void print_particles(Particle* particles, int n)
{
	for (int i = 0; i < n; ++i)
	{
		printf("Position: (%.2f, %.2f, %.2f) Velocity: (%.2f, %.2f, %.2f) State: %d, hash_value: %u\n", 
			particles[i].x,
			particles[i].y,
			particles[i].z,
			particles[i].v_x,
			particles[i].v_y,
			particles[i].v_z,
			particles[i].state,
			hash_particles(&particles[i]));
	}
}

void print_students(Student * students, int n)
{
	for (int i = 0; i < n; ++i)
	{
		printf("Age: %d GPA: %.2f Name: %s, hash_value: %u\n",
			students[i].age,
			students[i].gpa,
			students[i].name,
			hash_students(&students[i])); 
	}
}


int equal_particles(void * p1, void* p2)
{
	Particle particle1 = *(Particle *)p1;
	Particle particle2 = *(Particle *)p2;

	if(	particle1.x == particle2.x &&
		particle1.y == particle2.y &&
		particle1.z == particle2.z &&
		particle1.v_x == particle2.v_x &&
		particle1.v_y == particle2.v_y &&
		particle1.v_z == particle2.v_z &&
		particle1.state == particle2.state)
		return 1;
	else
		return 0;
}

int equal_students(void * s1, void * s2)
{
	Student student1 = *(Student *)s1;	
	Student student2 = *(Student *)s2;

	if(student1.age == student2.age &&
		student1.gpa == student2.gpa &&
		equal_words(student1.name,student2.name))
		return 1;
	else
		return 0;
}








int uniq(void * f, int n, int sz, int (*equals)(void *, void *), unsigned int (*h)(const void *key))
{
	void * data [n_bins];
	for (int i = 0; i < n_bins; ++i)
		data[i]=NULL;

	int n_unique_elem=0;

	for (int i = 0; i < n; ++i)
	{
		unsigned int index = h(f+i*sz);

		//In case no other element has had this has value, save it in final list
		if(data[index]==NULL)
		{
			memcpy(f+n_unique_elem*sz, f+i*sz,sz);
			data[index] = (void*)&f[n_unique_elem*sz];
			n_unique_elem++;
		}

		//In case another element has the same has value
		//we need to check that they are indeed equal elements
		else
		{
			//Different element with same hash value
			if(!equals(data[index],&f[i*sz]))
			{
				printf("Undesired collision!\n");
				memcpy(f+n_unique_elem*sz, f+i*sz,sz);
				n_unique_elem++;
			}
			//else
				//printf("Repeated\n");
		}
	}
	return n_unique_elem;

}

int main()
{



	Particle * particles = get_particles();
	Student * students = get_students();

	
	printf("Original List:\n");
	print_particles(particles,10);
	

	printf("Unique'd List:\n");
	int n_unique_particles = uniq(particles, 10, sizeof(Particle), &equal_particles, &hash_particles);
	print_particles(particles,n_unique_particles);

	printf("Original List:\n");
	print_students(students,10);
	printf("Unique'd List:\n");
	int n_unique_students = uniq(students, 10, sizeof(Student), &equal_students, &hash_students);
	print_students(students,n_unique_students);	

}