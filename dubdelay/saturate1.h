class saturate1
{
public:
	saturate1(){
		t = 1.0f;
		div = 1.0f / 65535.0f;
		p = (1-t)*1.5f;
	}
	~saturate1(){}
	
private:
	float t, div, p;

	inline float sigmoid(float x) 
	{ 
		if(fabs(x)<1) 
			return x*(1.5f - 0.5f*x*x); 
		else 
			return x > 0.f ? 1.f : -1.f; 
	} 

public:
	void set(float th)
	{
		t = th;
		p = (1-t)*1.5f;
	}
	inline float saturate(float x) 
	{ 
		x *= div;
	    if(fabs(x)<t) 
	        return x * 65535.0f; 
	    else 
	    { 
	        if(x > 0.f) 
	            return (t + (1.f-t)*sigmoid((x-t)*p)) * 65535.0f;
	        else 
	            return (-(t + (1.f-t)*sigmoid((-x-t)*p))) * 65535.0f; 
	    } 
	}  
float saturate2(float x)
{
	x *= div;
	if(fabs(x)) return x * 65535.0f;
	else{
		if(x > 0.f) return (t + (1.f-t)*tanhf((x-t)/(1-t))) * 65535.0f;
		else return -(t + (1.f-t)*tanhf((-x-t)/(1-t))) * 65535.0f;
	}
}
	
};