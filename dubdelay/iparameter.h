class IParameter
{
public:
	IParameter()
	{
		IsOn = false;
		incr = 0.0f;
		direction = 2;
		target = current = 0.0f;
		step = 1;
		inertiaLen = 0;
	}
	~IParameter(){}
	
	void Jump(float value)
	{
		IsOn = false;
		current = target = value;
	}
	void Init(float value, int step, int len)
	{	
		inertiaLen = len;
		target = current = value;
		step = step;
		incr = 0.0f;
		IsOn = false;
	}
	
	void operator= (float value)
	{
		target = value;
		incr = ( (target - current) / inertiaLen ) * step;
		if(target > current) direction = 0;//up
		else direction = 1;//down
		
		if(target != current) IsOn = true;
	}

	float& Get(){return current;}
	
	void SetLen(int len)
	{
		if(len != inertiaLen)
		{
			inertiaLen = len;
			incr = ( (target - current) / inertiaLen ) * step;
			if(target > current) direction = 0;//up
			else direction = 1;//down
		}
		
	}
	void Work() // call once every step
	{
		if(IsOn)
		{
			switch(direction)
			{
				case 0:
				current += incr;
				if(current > target){
					current = target;
					IsOn = false;
				}
				break;
				case 1:
				current += incr;
				if(current < target){
					current = target;
					IsOn = false;
				}
				break;
			}
		}
	}
private:
	bool IsOn;
	float target, current, incr;
	int inertiaLen, step, direction;
};