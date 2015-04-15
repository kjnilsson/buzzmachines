class AHDEnvelope
{
public:
	virtual void	SetEnvParams(int envc, int envl, float enva, float envd);//cound, length, attackamount, decay amount,
	float			Envelope(int envc); // * the output sample by what this function returns.
	virtual void	Init();
	virtual void	CheckActive();

public:
	int AtkLen, DecLen, DecStart;
	//int AtkCnt, DecCnt;
	int EnvLen;
	bool AtkPhase, DecPhase;


public:
	int C_Env;


};

void AHDEnvelope::SetEnvParams(int envc, int envl, float enva, float envd)
{
	EnvLen = envl;
	AtkLen = (int)(envl * enva);	
	DecLen = (int)(envl * envd);
	DecStart = EnvLen - DecLen;

	//AtkPhase = DecPhase = false;
/*
	if (C_Env <= AtkLen && AtkLen > 0) 
		AtkPhase = true;
	else AtkPhase = false;


	if (C_Env > DecStart && DecLen > 0)
		DecPhase = true;
	else DecPhase = false;
	*/

};

void AHDEnvelope::CheckActive()
{
	if (C_Env <= AtkLen && AtkLen > 0) 
		AtkPhase = true;
	else AtkPhase = false;


	if (C_Env > DecStart && DecLen > 0)
		DecPhase = true;
	else DecPhase = false;
}

void AHDEnvelope::Init()
{
	AtkLen = DecLen = 0;
	AtkPhase = DecPhase = false;
	C_Env = 0;

}

float AHDEnvelope::Envelope(int envc)
{
	float c, d;

	c = 1.0f;
	d = 1.0f;

	if (AtkPhase){
		c = (1.0f / AtkLen) * C_Env;
	}
	
	if (DecPhase){
		d = (1.0f / DecLen) * (DecLen - (C_Env - DecStart)); // 
	}

	c = c * d;

	C_Env++;

	return c;
	
};

/*
float AttackEnv(int al, int ca)
{
	float c;

	c = 1.0f / al * ca;

	return c;
}

float DecayEnv(int dl, int cd)
{
	float c;

	c = 1.0f / dl * (dl - cd);

	return c;
}
*/