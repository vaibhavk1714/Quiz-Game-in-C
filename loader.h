typedef struct node
{
	char q_filename[100];
	char a_filename[100];
	struct node *link;
} nd;

nd *insert_rear(nd *f, FILE **loader)
{
	nd* temp=NULL,*last=NULL;
    while(temp==NULL)
        temp=(nd*)malloc(sizeof(nd));
    fscanf(*loader, "%s%s",temp->q_filename, temp->a_filename);
    if(f==NULL)
    {
    	temp->link = temp;
        return temp;
    }
    for(last=f;last->link != f;last = last->link);
    last->link=temp;
    temp->link = f;
    return f;
}

nd *nodeinfo_loader(nd *f)
{
	FILE *loader = NULL;
	loader = fopen("filenames.txt", "r");
	int n;
	fscanf(loader, "%d", &n);
	for(int i = 0;i < n;i ++)
		f = insert_rear(f, &loader);
	fclose(loader);
	//printf("\n%x\n%x",f,f->link);
	return f;
}


void fr(nd *f)
{
	nd *temp = f, *cycle = f;
	do
	{
		temp = f->link;
		free(f);
		f = temp;
	}while (f != cycle);
}

