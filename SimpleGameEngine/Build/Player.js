class Player extends EntityBase
{
	constructor()
	{
		super()
		this.DrawRect0 = new DrawRect( this,50,50,"#ff0000" )
		this.ClampToScreen0 = new ClampToScreen( this,25 )
		this.Accel0 = new Accel( this,1,1,0.5,0.3 )
	}

	EntityStart()
	{
		this.DrawRect0.OnStart()
		this.ClampToScreen0.OnStart()
		this.Accel0.OnStart()
	}

	EntityUpdate()
	{
		this.DrawRect0.OnUpdate()
		this.ClampToScreen0.OnUpdate()
		this.Accel0.OnUpdate()
	}

	EntityDraw()
	{
		this.DrawRect0.OnDraw()
		this.ClampToScreen0.OnDraw()
		this.Accel0.OnDraw()
	}

	EntityCollision()
	{
		this.DrawRect0.OnCollision()
		this.ClampToScreen0.OnCollision()
		this.Accel0.OnCollision()
	}

	EntityDestroy()
	{
		this.DrawRect0.OnDestroy()
		this.ClampToScreen0.OnDestroy()
		this.Accel0.OnDestroy()
	}

}