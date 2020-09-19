// Accel
// Accelerate an object in a direction.
// xVel yVel xAccel yAccel

class Accel extends ModuleBase
{
	constructor( parent,xVel,yVel,xAccel,yAccel )
	{
		super( parent )
		this.xVel = xVel
		this.yVel = yVel
		this.xAccel = xAccel
		this.yAccel = yAccel
	}
	
	OnUpdate()
	{
		this.xVel += this.xAccel
		this.yVel += this.yAccel
		
		this.parent.x += this.xVel
		this.parent.y += this.yVel
	}
	
	OnCollision()
	{
		this.xVel = 0.0
		this.yVel = 0.0
	}
}