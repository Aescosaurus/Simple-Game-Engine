// ClampToScreen
// Keeps object on screen.
// buffer

class ClampToScreen extends ModuleBase
{
	constructor( parent,buffer )
	{
		super( parent )
		this.buffer = buffer
	}
	
	OnUpdate()
	{
		if( this.parent.x < 0 + this.buffer )
		{
			this.parent.x = 0 + this.buffer
			this.parent.EntityCollision()
		}
		if( this.parent.x > gfx.scrWidth - this.buffer )
		{
			this.parent.x = gfx.scrWidth - this.buffer
			this.parent.EntityCollision()
		}
		if( this.parent.y < 0 + this.buffer )
		{
			this.parent.y = 0 + this.buffer
			this.parent.EntityCollision()
		}
		if( this.parent.y > gfx.scrHeight - this.buffer )
		{
			this.parent.y = gfx.scrHeight - this.buffer
			this.parent.EntityCollision()
		}
	}
}
